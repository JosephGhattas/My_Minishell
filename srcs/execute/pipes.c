/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:23:18 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:23:20 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	exec_child(t_command *cmd, int in_fd, int out_fd)
{
	char	*path;

	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	if (is_builtin(cmd->argv[0]))
		exit(run_builtin(cmd->argc, cmd->argv, &cmd->envp));
	path = find_path(cmd->argv[0], cmd->envp);
	execve(path, cmd->argv, cmd->envp);
	free(path);
	perror("execve");
	exit(1);
}

int	execute_pipeline(t_command *cmd)
{
	int		pipefd[2];
	int		in_fd = 0;
	pid_t	pid;

	while (cmd->next)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			close(pipefd[0]);
			exec_child(cmd, in_fd, pipefd[1]);
		}
		close(pipefd[1]);
		if (in_fd != 0)
			close(in_fd);
		in_fd = pipefd[0];
		cmd = cmd->next;
	}
	pid = fork();
	if (pid == 0)
		exec_child(cmd, in_fd, 1);
	close(in_fd);
	while (wait(NULL) > 0)
		;
	return (0);
}
