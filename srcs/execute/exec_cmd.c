/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:28 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/10 19:38:43 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	execute_builtin(t_ast_node *cmd, t_env_list **env)
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

void handle_execve_error(char *cmd)
{
    if (errno == EACCES || errno == EPERM || errno == EISDIR || 
        errno == ENOEXEC || errno == ETXTBSY)
		{
        perror(cmd);
        free(cmd);
        exit(126);
    }
    else if (errno == ENOENT || errno == ENOTDIR)
	{
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        free(cmd);
        exit(127);
    }
    else
	{
        perror(cmd);
        free(cmd);
        exit(127);
    }
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
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_array(envp);
		exit(127);
	}
	setup_signals_exec();
	execve(path, cmd->args, envp);
	free_array(envp);
	handle_execve_error(path);
}

static int	execute_external(t_ast_node *cmd, t_env_list *env)
{
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		return (1);
	}
	if (pid == 0)
		child_process(cmd, env);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_command_node(t_ast_node *cmd, t_env_list **env)
{
	if (!cmd || !cmd->argc || !cmd->args[0])
		return (1);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, env));
	else
		return (execute_external(cmd, *env));
}
