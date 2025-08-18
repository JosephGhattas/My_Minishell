/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:46:52 by jgh               #+#    #+#             */
/*   Updated: 2025/08/18 15:47:52 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_heredoc_child(t_redir *redir, t_env_list *env)
{
	int	fd;

	g_sig = 0;
	fd = open_heredoc_fd(redir->filename);
	if (fd == -1)
		exit(1);
	setup_signals_heredoc();
	heredoc_read_loop(redir, env, fd);
	close(fd);
	exit(0);
}

int	wait_for_heredoc_child(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1 && errno == EINTR && g_sig == SIGINT)
	{
		kill(pid, SIGINT);
		signal(SIGINT, SIG_IGN);
		if (waitpid(pid, status, 0) == -1)
			return (-1);
	}
	return (0);
}

int	interpret_child_status(int status, t_redir *redir)
{
	if (g_sig == SIGINT || (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		unlink_and_free_filename(redir);
		return (130);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		unlink_and_free_filename(redir);
		return (WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		unlink_and_free_filename(redir);
		return (128 + WTERMSIG(status));
	}
	return (0);
}

pid_t	heredoc_setup_and_fork(void (**old_sigint)(int),
		void (**old_sigquit)(int))
{
	*old_sigint = signal(SIGINT, handle_parent_sigint);
	*old_sigquit = signal(SIGQUIT, SIG_IGN);
	return (fork());
}

int	heredoc_parent_wait_and_restore(pid_t pid,
		void (*old_sigint)(int), void (*old_sigquit)(int),
		t_redir *redir)
{
	int	status;

	if (wait_for_heredoc_child(pid, &status) == -1)
	{
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		unlink_and_free_filename(redir);
		return (1);
	}
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	return (interpret_child_status(status, redir));
}
