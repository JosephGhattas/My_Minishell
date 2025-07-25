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
}

int	main(int argc, char **argv, char **envp)
{
	t_env_list *env;
    char        *input;
    // t_command   *cmd;

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
		if (*input)
			add_history(input);
		parse_input(input, env);
	    // cmd = parse_input(input, my_env);
        // if (!cmd)
        // {
        //     free(input);
        //     break ;
        // }
        
        // if (handle_all_heredocs(cmd) != 0)
        // {
        //     free_command(cmd);
        //     free(input);
        //     break ;
        // }
	    // execute_command(cmd);
	    // free_command(cmd);
	    free(input);
        if (g_sig != 0)
		{
			env->exit_status = 128 + g_sig;
			g_sig = 0;
		}
    }
	free_env_list_full(env);
    return (0);
}
