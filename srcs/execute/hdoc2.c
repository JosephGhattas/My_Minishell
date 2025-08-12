/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:14:17 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/12 15:14:18 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static int	prepare_heredoc_file(t_redir *redir)
{
	int	fd;

	redir->filename = generate_heredoc_filename();
	if (!redir->filename)
		return (-1);
	fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (fd_failed(redir, true));
	return (fd);
}

static int	process_heredoc_line(char **line, t_redir *redir,
		t_env_list *env, int fd)
{
	char	*expanded_line;

	if (!redir->heredoc_quoted)
	{
		expanded_line = expand_heredoc_line(*line, env);
		free(*line);
		if (!expanded_line)
		{
			close(fd);
			unlink(redir->filename);
			return (fd_failed(redir, false));
		}
		*line = expanded_line;
	}
	write(fd, *line, ft_strlen(*line));
	write(fd, "\n", 1);
	free(*line);
	return (0);
}

static int	fill_heredoc_file(int fd, t_redir *redir, t_env_list *env)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd);
			unlink(redir->filename);
			return (fd_failed(redir, false));
		}
		if (ft_strcmp(line, redir->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (process_heredoc_line(&line, redir, env, fd) != 0)
			return (-1);
	}
	return (0);
}

int	create_heredoc_file(t_redir *redir, t_env_list *env)
{
	int	fd;

	fd = prepare_heredoc_file(redir);
	if (fd < 0)
		return (fd);
	if (fill_heredoc_file(fd, redir, env) != 0)
		return (-1);
	close(fd);
	return (0);
}
