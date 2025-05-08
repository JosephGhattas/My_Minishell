#include "minishell.h"

int	execute_command(t_command *cmd)
{
    pid_t	pid;
    int		status;

    if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd->argc, cmd->argv, cmd->envp));
	pid = fork();
	if (pid == 0)
    {
		execve(find_path(cmd->argv[0]), cmd->argv, cmd->envp); // Implement find_path
		perror("execve");
		exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
}
