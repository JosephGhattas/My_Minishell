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

int	main(int argc, char **argv, char **envp)
{
	char	    **my_env;
    char        *input;
    t_command   *cmd;

    my_env = copy_env(envp);
    (void)argc;
    (void)argv;
	printbanner();
    while (1)
    {
        
	    input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		printf("input: %s\n", input);
		//parse_input(input, my_env);
	    cmd = parse_input(input, my_env);
        if (!cmd)
        {
            free(input);
            continue ;
        }
        if (handle_all_heredocs(cmd) != 0)
        {
            free_command(cmd);
            free(input);
            break ;
        }
	    execute_command(cmd);
	    free_command(cmd);
	    free(input);
    }
	free_env(my_env);

    // Simulated parsed command: env
    // t_command cmd1 = {
    //     .argv = (char *[]){"env", NULL},
    //     .argc = 1,
    //     .infile = NULL,
    //     .outfile = NULL,
    //     .append = 0,
    //     .envp = my_env,
    //     .next = NULL
    // };
    /*// Simulate: cat < input.txt | grep hello | wc -l > out.txt
    t_command c1 = { .argv = (char *[]){"cat", NULL}, .infile = "input.txt", .next = &c2 };
    t_command c2 = { .argv = (char *[]){"grep", "hello", NULL}, .next = &c3 };
    t_command c3 = { .argv = (char *[]){"wc", "-l", NULL}, .outfile = "out.txt", .append = 0 };
    */

    // execute_command(&cmd1);
	
    return (0);
}



/*
parsing plan:
detect imp content(pipes, quotes, double quotes, heredoc, redirect
	Handle environment variables ($ followed by a sequence of characters) which
	should expand to their values.
	• Handle $? which should expand to the exit status of the most recently executed
	foreground pipeline.
	&& history )
rank in a tree by priority
execute them 
*/