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

char	*my_getenv(char *name, char **envp)
{
	int		i;
	int		len;

	i = 0;
	while (envp[i])
	{
		len = 0;
		while (envp[i][len] && envp[i][len] != '=')
			len++;
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*env_path;
	char	*full_path;
	char	*temp;
	int		i;

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
					free_array(paths);
					return (full_path);
				}
				free(full_path);
			}
		}
	}
	free_array(paths);
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

int	setup_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		fd = open_redirection_file(redir);
		if (fd == -1)
			return (1);
		if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
		}
		else if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
		}
		close(fd);
		redir = redir->next;
	}
	return (0);
}
