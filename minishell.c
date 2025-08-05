/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:24:15 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:36:32 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(int argc, char **argv, t_env_list **env_list, char **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("ERROR: too many arguments\n", 2);
		exit(127);
	}
	cheaking_env(&(*env_list), env);
	// update_exit_status(env_list, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_list *env;
    char        *input;
    t_ast_node   *tree;
	int		exit_status;
	t_redir	*redir;

	exit_status = 0;
	printbanner();
    init(argc, argv, &env, envp);
    while (1)
    {
		setup_signals_prompt();
		input = readline("Minishell$ ");
	    // input = read_complete_input();

		if (!input)
			break ;
		if (!*input || is_only_whitespace(input))
        {
            free(input);
            continue;
        }
		add_history(input);
	    if (detect_syntax_errors(input))
        {
            free(input);
            continue;
        }
		tree = parse_input(input, env);
		free(input);
        if (!tree)
        {
			free_env_list_full(env);
            exit (EXIT_FAILURE);
        } 
		redir = tree->redirections;
		if (setup_all_heredocs(tree) != 0)
		{
			perror("Heredoc interrupted");
			free_env_list_full(env);
			free_ast(tree);
			exit(EXIT_FAILURE);
		}
		exit_status = execute_ast(tree, &env);
		update_exit_status(&env, exit_status);
		free_ast(tree);
        if (g_sig != 0)
		{
			env->exit_status = 128 + g_sig;
			g_sig = 0;
		}
    }
	free_redir_list(redir);
	free_env_list_full(env);
	rl_clear_history();
	rl_free_line_state();
    return (0);
}
