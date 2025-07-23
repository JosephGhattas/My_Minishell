/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:23:10 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:23:11 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	setup_heredoc(t_command *cmd)
{
	int		pipefd[2];
	char	*line;

	if (!cmd->heredoc_delim)
		return (0);
	if (pipe(pipefd) == -1)
		return (perror("heredoc pipe"), 1);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, cmd->heredoc_delim) == 0)
			break ;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	cmd->heredoc_fd = pipefd[0];
	return (0);
}

int	handle_all_heredocs(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_delim)
		{
			if (setup_heredoc(cmd) != 0)
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
