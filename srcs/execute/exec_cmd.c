/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:54:16 by jgh               #+#    #+#             */
/*   Updated: 2025/08/04 21:05:01 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	execute_builtin(t_ast_node *cmd, t_env_list *env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (1);
	if (setup_redirections(cmd->redirections) != 0)
		status = 1;
	else
		status = run_builtin(cmd->argc, cmd->args, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

static void	child_process(t_ast_node *cmd, t_env_list *env)
{
	char	**envp;
	char	*path;

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

static int	execute_external(t_ast_node *cmd, t_env_list *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		child_process(cmd, env);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_command_node(t_ast_node *cmd, t_env_list *env)
{
	if (!cmd || !cmd->argc || !cmd->args[0])
		return (1);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, env));
	else
		return (execute_external(cmd, env));
}
