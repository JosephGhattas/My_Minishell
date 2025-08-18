/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:16:20 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 17:16:22 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_heredoc_fd(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);
	return (fd);
}

void	unlink_and_free_filename(t_redir *redir)
{
	if (redir->filename)
	{
		unlink(redir->filename);
		free(redir->filename);
		redir->filename = NULL;
	}
}

int	heredoc_handle_null_line(t_redir *redir, int fd)
{
	if (g_sig == SIGINT)
	{
		close(fd);
		unlink(redir->filename);
		exit(130);
	}
	ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
	ft_putstr_fd("end-of-file (wanted `", 2);
	ft_putstr_fd(redir->delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return (1);
}

void	heredoc_process_failure(char *line, t_redir *redir, int fd)
{
	free(line);
	close(fd);
	unlink(redir->filename);
	exit(1);
}

void	heredoc_read_loop(t_redir *redir, t_env_list *env, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (heredoc_handle_null_line(redir, fd))
				break ;
		}
		else if (ft_strcmp(line, redir->delimiter) == 0)
		{
			free(line);
			break ;
		}
		else if (process_heredoc_line(line, redir, env, fd) != 0)
		{
			heredoc_process_failure(line, redir, fd);
		}
		else
			free(line);
	}
}
