/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:23:34 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:22:58 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin( char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	run_builtin(int argc, char **argv, t_env_list *envp)
{
	(void)envp;
	if (!argv || !argv[0])
		return (1);
	if (!ft_strcmp(argv[0], "cd"))
		return (my_cd(argc, argv, envp));
	else if (!ft_strcmp(argv[0], "echo"))
		return (my_echo(argc, argv));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (my_pwd(envp));
	// else if (!ft_strcmp(argv[0], "export"))
	// 	return (my_export(argc, argv, envp));
	// else if (!ft_strcmp(argv[0], "unset"))
	// 	return (my_unset(argc, argv, envp));
	// else if (!ft_strcmp(argv[0], "env"))
	// 	return (my_env(argc, argv, envp));
	else if (!ft_strcmp(argv[0], "exit"))
	{
		my_exit(argc, argv);
		return (0);
	}
	return (1);
}
