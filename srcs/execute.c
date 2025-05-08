/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:23:42 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:41:35 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

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
	char	*path;
	char	*full;
	int		i;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = ft_split(my_getenv("PATH", envp), ':');
	if (!paths)
		return (ft_strdup(cmd));
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		full = ft_strjoin(path, cmd);
		free(path);
		if (access(full, F_OK | X_OK) == 0)
			return (free_array(paths), full);
		free(full);
	}
	return (free_array(paths), ft_strdup(cmd));
}

static void	redirect_io(t_command *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror("infile");
			exit(1);
		}
		dup2(fd, 0);
		close(fd);
	}
	if (cmd->outfile)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT
				|(cmd->append ? O_APPEND : O_TRUNC), 0644);
		if (fd < 0)
		{
			perror("outfile");
			exit(1);
		}
		dup2(fd, 1);
		close(fd);
	}
}

int	execute_command(t_command *cmd)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd->argc, cmd->argv, &cmd->envp));
	pid = fork();
	if (pid == 0)
	{
		redirect_io(cmd);
		path = find_path(cmd->argv[0], cmd->envp);
		execve(path, cmd->argv, cmd->envp);
		free(path);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
