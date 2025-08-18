/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:28 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 14:57:18 by jgh              ###   ########.fr       */
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
		status = run_builtin(cmd, cmd->argc, cmd->args, env);
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
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_array(envp);
		exit(127);
	}
	setup_signals_exec();
	execve(path, cmd->args, envp);
	free_array(envp);
	handle_execve_error(cmd->args[0], path);
}

static int	get_exit_code_from_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(1, "\n", 1);
		return (128 + sig);
	}
	else if (WIFEXITED(status))
	{
		return (WEXITSTATUS(status));
	}
	return (1);
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
	return (get_exit_code_from_status(status));
}

int	execute_command_node(t_ast_node *cmd, t_env_list **env)
{
	if (!cmd || !cmd->args)
		return (1);
	if (cmd->argc == 0)
		return (0); 
	if (cmd->args[0][0] == '\0')
    {
        ft_putstr_fd("minishell: '': command not found\n", STDERR_FILENO);
        return (127);
    }
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, env));
	else
		return (execute_external(cmd, *env));
}
