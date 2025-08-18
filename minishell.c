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

static void	init(int argc, char **argv, t_env_list **env_list, char **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("ERROR: too many arguments\n", 2);
		exit(127);
	}
	cheaking_env(env_list, env);
}

int	handle_execution(t_ast_node *tree, t_env_list **env)
{
	int	exit_status;

	if (setup_all_heredocs(tree, *env) != 0)
	{
		if (g_sig == SIGINT)
			return (130);
		if (setup_all_heredocs(tree, *env) == 130)
			return (130);
		perror("Heredoc interrupted");
		return (1);
	}
	exit_status = execute_ast(tree, env);
	update_exit_status(env, exit_status);
	return (exit_status);
}

static int	process_input(t_env_list **env)
{
	char		*input;
	t_ast_node	*tree;
	int			exit_status;

	setup_signals_prompt();
	input = readline("Minishell$ ");
	if (!input)
		return (-1);
	if (!*input || is_only_whitespace(input))
		return (free(input), 0);
	if (g_sig != 0)
	{
		update_exit_status(env, 128 + g_sig);
		g_sig = 0;
	}
	add_history(input);
	if (detect_syntax_errors(input))
		return (free(input), 2);
	tree = parse_input(input, *env);
	free(input);
	if (!tree)
		return (-1);
	exit_status = handle_execution(tree, env);
	free_ast(tree);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_list	*env;
	int			status;
	int			last_exit_code;

	printbanner();
	init(argc, argv, &env, envp);
	while (1)
	{
		if (g_sig != 0)
		{
			update_exit_status(&env, 128 + g_sig);
			g_sig = 0;
		}
		status = process_input(&env);
		if (status == -1)
		{
			last_exit_code = get_exit_status(env);
			printf("exit\n");
			break ;
		}
	}
	free_env_list_full(env);
	rl_clear_history();
	return (last_exit_code);
}
