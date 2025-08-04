/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:18:39 by jgh               #+#    #+#             */
/*   Updated: 2025/08/04 19:57:03 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	left_pipe(t_ast_node *node, t_env_list *env, int *fd, pid_t *left_pid)
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

int	right_pipe(t_ast_node *node, t_env_list *env, int *fd, pid_t *right_pid)
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

int	execute_pipe(t_ast_node *node, t_env_list *env)
{
	int		fd[2];
	pid_t	pids[2];
	int		status;

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
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_command_node(t_ast_node *cmd, t_env_list *env)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	status = 0;
	if (!cmd || !cmd->argc || !cmd->args[0])
		return (1);
	if (is_builtin(cmd->args[0]))
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		if (setup_redirections(cmd->redirections) != 0)
			status = 1;
		else
			status = run_builtin(cmd->argc, cmd->args, env);
		dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (setup_redirections(cmd->redirections) != 0)
				exit(EXIT_FAILURE);
			envp = env_list_to_envp(env);
			if (!envp)
				exit(125);
			path = find_path(cmd->args[0], envp);
			if (!path)
			{
				free_array(envp);
				exit(127);
			}
			setup_signals_exec();
			execve(path, cmd->args, envp);
			free(path);
			free_array(envp);
			perror("execve");
			exit(126);
		}
		else
			waitpid(pid, &status, 0);
	}
	return (WEXITSTATUS(status));
}

int	execute_ast(t_ast_node *node, t_env_list *env)
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
