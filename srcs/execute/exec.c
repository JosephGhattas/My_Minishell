/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:58 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/06 10:33:58 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	left_pipe(t_ast_node *node, t_env_list **env, int *fd, pid_t *left_pid)
{
	*left_pid = fork();
	if (*left_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (*left_pid == 0)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd[1]);
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
		exit(execute_ast(node->left, env));
	}
	return (0);
}

int	right_pipe(t_ast_node *node, t_env_list **env, int *fd, pid_t *right_pid)
{
	*right_pid = fork();
	if (*right_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (*right_pid == 0)
	{
		close(fd[1]);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd[0]);
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		exit(execute_ast(node->right, env));
	}
	return (0);
}

static int	start_pipe_children(t_ast_node *node, t_env_list **env,
									int *fd, pid_t *pids)
{
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	if (left_pipe(node, env, fd, &pids[0]) == -1)
		return (close(fd[0]), close(fd[1]), 1);
	if (right_pipe(node, env, fd, &pids[1]) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		kill(pids[0], SIGKILL);
		waitpid(pids[0], NULL, 0);
		return (1);
	}
	close(fd[0]);
	close(fd[1]);
	return (0);
}

int	execute_pipe(t_ast_node *node, t_env_list **env)
{
	int		fd[2];
	pid_t	pids[2];
	int		status;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	if (start_pipe_children(node, env, fd, pids) == 1)
		return (signal(SIGINT, old_sigint), signal(SIGQUIT, old_sigquit), 1);
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], &status, 0);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_ast(t_ast_node *node, t_env_list **env)
{
	if (!node)
		return (1);
	if (node->type == NODE_PIPE)
		return (execute_pipe(node, env));
	else if (node->type == NODE_COMMAND)
		return (execute_command_node(node, env));
	else
		return (1);
}
