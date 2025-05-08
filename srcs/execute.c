#include "../minishell.h"

void	free_array(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*my_getenv(char *name, char **envp)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		str = ft_substr(envp[i], 0, j);
		if (ft_strcmp(str, name) == 0)
		{
			free(str);
			return (envp[i] + j + 1);
		}
		free(str);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	int		i;
	char	*exec;
	char	**allpaths;
	char	*path;
	char	**cmd1;

	i = -1;
	allpaths = ft_split(my_getenv("PATH", envp), ':');
	cmd1 = ft_split(cmd, ' ');
	while (allpaths[++i])
	{
		path = ft_strjoin(allpaths[i], "/");
		exec = ft_strjoin(path, cmd1[0]);
		free(path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_array(cmd1);
			free_array(allpaths);
			return (exec);
		}
		free(exec);
	}
	free_array(allpaths);
	free_array(cmd1);
	return (cmd);
}

int	execute_command(t_command *cmd)
{
    pid_t	pid;
    int		status;

    if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd->argc, cmd->argv, &cmd->envp));
	pid = fork();
	if (pid == 0)
    {
		execve(find_path(cmd->argv[0], cmd->envp), cmd->argv, cmd->envp);
		perror("execve");
		exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
}
