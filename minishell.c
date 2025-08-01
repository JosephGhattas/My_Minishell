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

bool has_unclosed_quotes(const char *input)
{
	char quote_type = 0; 
	int i = 0;
	while (input[i])
	{
    	if ((input[i] == '"' || input[i] == '\'')) {
        	if (quote_type == 0) {
            	// Start of a quote
            	quote_type = input[i];
        	} else if (quote_type == input[i]) {
            	// Closing the current quote
            	quote_type = 0;
        	}
    	}
    	i++;
	}
	return (quote_type);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_list *env;
    char        *input;
    t_ast_node   *tree;

	printbanner();
    init(argc, argv, &env, envp);
    while (1)
    {
        setup_signals_prompt();
	    input = readline("CJminishell$ ");
		if (!input)
		{
            free(input);
			free_env_list_full(env);
			exit(EXIT_FAILURE);
		}
		if (has_unclosed_quotes(input))// added by charbel
        {
            fprintf(stderr, "Syntax error: unclosed quote detected.\n");
            free(input);
            continue;
        }
		if (*input)
			add_history(input);
		tree = parse_input(input, env);
		free(input);
        if (!tree)
        {
			free_env_list_full(env);
            exit (EXIT_FAILURE);
        }  
		if (setup_all_heredocs(tree) != 0)
		{
			perror("Heredoc interrupted");
			free_env_list_full(env);
			free_ast(tree);
			exit(EXIT_FAILURE);
		}
		execute_ast(tree, env);
        if (g_sig != 0)
		{
			env->exit_status = 128 + g_sig;
			g_sig = 0;
		}
    }
	free_env_list_full(env);
	free_ast(tree);
    return (0);
}
