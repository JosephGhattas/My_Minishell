#include "../minishell.h"

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
	dup2(pipefd[0], 0);
	close(pipefd[0]);
	return (0);
}