/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:14:17 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 09:05:22 by jgh              ###   ########.fr       */
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

static int	process_heredoc_line(char *line, t_redir *redir,
				t_env_list *env, int fd)
{
	char	*expanded_line;

	if (!redir->heredoc_quoted)
	{
		expanded_line = expand_heredoc_line(line, env);
		if (!expanded_line)
			return (-1);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

static int	fill_heredoc_file(int fd, t_redir *redir, t_env_list *env)
{
	char	*line;

	setup_signals_heredoc();
	while (1)
	{
		if (g_sig == SIGINT)
		{
			free(line);
			break ;
		}
		line = readline("> ");
		if (g_sig == SIGINT)
		{
			free(line);
			break ;
		}
		if (!line)
		{
			close(fd);
			setup_signals_prompt();
			return (fd_failed(redir, false));
		}
		if (ft_strcmp(line, redir->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (process_heredoc_line(line, redir, env, fd) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	setup_signals_prompt();
	if (g_sig == SIGINT)
	{
		close(fd);
		unlink(redir->filename);
		free(redir->filename);
		redir->filename = NULL;
		// rl_cleanup_after_signal();
		// rl_replace_line("", 0);
		// rl_on_new_line();
		return (-1);
	}
	return (0);
}

// static void	handle_heredoc_child(t_redir *redir, t_env_list *env)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
// 	if (fd == -1)
// 		exit(1);
// 	signal(SIGINT, sig_handler_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (g_sig == SIGINT)  // Check for Ctrl+C first
// 		{
// 			free(line);
// 			break;
// 		}
// 		if (!line)
// 			break ;
// 		if (ft_strcmp(line, redir->delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (process_heredoc_line(line, redir, env, fd) != 0)
// 		{
// 			free(line);
// 			close(fd);
// 			exit(1);
// 		}
// 		free(line);
// 	}
// 	close(fd);
// 	if (g_sig == SIGINT)
// 		exit(130);
// 	exit(0);
// }

int	create_heredoc_file(t_redir *redir, t_env_list *env)
{
	int	fd;

	fd = prepare_heredoc_file(redir);
	if (fd < 0)
		return (fd);
	if (fill_heredoc_file(fd, redir, env) != 0)
	{
		if (g_sig == SIGINT)
			return (130);
		return (-1);
	}
	close(fd);
	return (0);
}

// int	create_heredoc_file(t_redir *redir, t_env_list *env)
// {
// 	pid_t	pid;
// 	int		status;
// 	void	(*old_sigint)(int);
// 	void	(*old_sigquit)(int);

// 	redir->filename = generate_heredoc_filename();
// 	if (!redir->filename)
// 		return (-1);
// 	old_sigint = signal(SIGINT, SIG_IGN);
// 	old_sigquit = signal(SIGQUIT, SIG_IGN);
// 	pid = fork();
// 	if (pid == -1)
// 		return (fd_failed(redir, false));
// 	if (pid == 0)
// 		handle_heredoc_child(redir, env);
// 	waitpid(pid, &status, 0);
// 	signal(SIGINT, old_sigint);
// 	signal(SIGQUIT, old_sigquit);
// 	if (WIFEXITED(status))
// 	{
// 		if (WEXITSTATUS(status) == 130)  // Heredoc interrupted by Ctrl+C
// 		{
// 			unlink(redir->filename);
// 			free(redir->filename);
// 			redir->filename = NULL;
// 			return (130);
// 		}
// 		if (WEXITSTATUS(status) != 0)  // Other errors
// 			return (-1);
// 	}
// 	return (0);
// }
