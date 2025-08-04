/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:22:30 by jgh               #+#    #+#             */
/*   Updated: 2025/08/04 17:39:31 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*generate_heredoc_filename(void)
{
	static int	counter = 0;
	char		*number;
	char		*name;
	char		*prefix;

	number = ft_itoa(counter++);
	if (!number)
		return (NULL);
	prefix = ft_strdup("/tmp/.heredoc_tmp_");
	if (!prefix)
		return (free(number), NULL);
	name = ft_strjoin(prefix, number);
	free(prefix);
	free(number);
	return (name);
}

int	fd_failed(t_redir	*redir)
{
	free(redir->filename);
	redir->filename = NULL;
	perror("open");
	return (-1);
}

int	create_heredoc_file(t_redir *redir)
{
	int		fd;
	char	*line;

	redir->filename = generate_heredoc_filename();
	if (!redir->filename)
		return (-1);
	fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (fd_failed(redir));
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->delimiter) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

int	setup_heredocs(t_redir *list)
{
	while (list)
	{
		if (list->type == TOKEN_HEREDOC && list->delimiter)
		{
			if (create_heredoc_file(list) != 0)
			{
				ft_putstr_fd("minishell: heredoc failed\n", 2);
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}

int	setup_all_heredocs(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (setup_heredocs(node->redirections) != 0)
			return (1);
	}
	if (node->left)
	{
		if (setup_all_heredocs(node->left) != 0)
			return (1);
	}
	if (node->right)
	{
		if (setup_all_heredocs(node->right) != 0)
			return (1);
	}
	return (0);
}
