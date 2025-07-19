/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:24:15 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/01 15:42:23 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

    my_env = copy_env(envp);
    (void)argc;
    (void)argv;
    while (1)
    {
        printbanner();
	    char *input = readline("minishell$ ");
	    t_command *cmd = parse_input(input, my_env); // <-- your parser returns this
	    execute_command(cmd);
	    free_command(cmd);
        free_env(my_env);
	    free(input);
    }

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
