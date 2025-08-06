/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:23:42 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:50:15 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../..//minishell.h"

char	*search_in_paths(char *cmd, char **paths)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (temp)
		{
			full_path = ft_strjoin(temp, cmd);
			free(temp);
			if (full_path)
			{
				if (access(full_path, F_OK | X_OK) == 0)
				{
					return (full_path);
				}
				free(full_path);
			}
		}
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*env_path;
	char	*full_path;

	paths = NULL;
	env_path = NULL;
	full_path = NULL;
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	env_path = my_getenv("PATH", envp);
	if (!env_path)
		return (ft_strdup(cmd));
	paths = ft_split(env_path, ':');
	if (!paths)
		return (ft_strdup(cmd));
	full_path = search_in_paths(cmd, paths);
	free_array(paths);
	if (full_path)
		return (full_path);
	return (ft_strdup(cmd));
}

int	open_redirection_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == TOKEN_HEREDOC)
		fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
		perror(redir->filename);
	return (fd);
}

int	dup2_with_check(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror("dup2");
		close(oldfd);
		return (1);
	}
	close(oldfd);
	return (0);
}

int	setup_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		fd = open_redirection_file(redir);
		if (fd == -1)
			return (1);
		if (redir->type == TOKEN_REDIR_IN
			|| redir->type == TOKEN_HEREDOC)
		{
			if (dup2_with_check(fd, STDIN_FILENO))
				return (1);
		}
		else if (redir->type == TOKEN_REDIR_OUT
			|| redir->type == TOKEN_REDIR_APPEND)
		{
			if (dup2_with_check(fd, STDOUT_FILENO))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
