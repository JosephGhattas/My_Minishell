/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:18:39 by jgh               #+#    #+#             */
/*   Updated: 2025/07/29 13:24:17 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_pipe(t_ast_node *node, t_env_list *env)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	left_pid = fork();
	if (left_pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(execute_ast(node->left, env));
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(execute_ast(node->right, env));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
	return (0);
}

int	execute_command_node(t_ast_node *cmd, t_env_list *env)
{
	pid_t	pid;
	int		status;
	char	**envp;

	if (!cmd || !cmd->argc || !cmd->args[0])
		return (1);
	if (is_builtin(cmd->args[0]))
		return (printf("builtin\n"));
		// return(run_builtin(cmd->argc, cmd->args, env_list_to_envp(env)));
	else
	{
		pid = fork();
		if (pid == 0)
		{
			setup_redirections(cmd->redirections);
			envp = env_list_to_envp(env);
			setup_signals_exec();
			execve(find_path(cmd->args[0], envp), cmd->args, envp);
			perror("execve");
			exit(127);
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
