/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:14:17 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 15:56:43 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_parent_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
}

char	*append_var(char *result, const char *input, int *i, t_env_list *env)
{
	char	*tmp;
	char	*new;

	tmp = expand_var(input, i, env);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	new = ft_strjoin(result, tmp);
	free(result);
	free(tmp);
	return (new);
}

int	process_heredoc_line(char *line, t_redir *redir,
				t_env_list *env, int fd)
{
	char	*expanded_line;

	if (!redir->heredoc_quoted)
	{
		expanded_line = expand_heredoc_line(line, env);
		if (!expanded_line)
			return (-1);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

int	create_heredoc_file(t_redir *redir, t_env_list *env)
{
	pid_t	pid;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	g_sig = 0;
	redir->filename = generate_heredoc_filename();
	if (!redir->filename)
		return (-1);
	pid = heredoc_setup_and_fork(&old_sigint, &old_sigquit);
	if (pid == -1)
	{
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		return (fd_failed(redir, false));
	}
	if (pid == 0)
	{
		handle_heredoc_child(redir, env);
		exit(0);
	}
	return (heredoc_parent_wait_and_restore(pid, old_sigint,
			old_sigquit, redir));
}
