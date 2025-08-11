/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/11 23:35:12 by jgh              ###   ########.fr       */
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

int	fd_failed(t_redir	*redir, bool open)
{
	free(redir->filename);
	redir->filename = NULL;
	if (open)
		perror("open");
	return (-1);
}

static char	*expand_heredoc_line(const char *input, t_env_list *env)
{
	char	*result;
	char	*temp;
	char	*new_result;
	int		i;
	bool	escape_next;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	escape_next = false;
	while (input[i])
	{
		if (escape_next)
		{
			temp = ft_char_to_str(input[i]);
			if (!temp)
				return (free(result), NULL);
			new_result = ft_strjoin(result, temp);
			free(result);
			free(temp);
			result = new_result;
			escape_next = false;
			i++;
		}
		else if (input[i] == '\\')
		{
			if (input[i + 1] == '\n')
				i += 2;
			else if (input[i + 1] != '\0')
			{
				escape_next = true;
				i++;
			}
			else
			{
				temp = ft_char_to_str(input[i]);
				if (!temp)
					return (free(result), NULL);
				new_result = ft_strjoin(result, temp);
				free(result);
				free(temp);
				if (!new_result)
					return (NULL);
				result = new_result;
				i++;
			}
		}
		else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
		{
			temp = expand_var(input, &i, env);
			if (!temp)
				return (free(result), NULL);
			new_result = ft_strjoin(result, temp);
			free(result);
			free(temp);
			result = new_result;
		}
		else
		{
			temp = ft_char_to_str(input[i]);
			if (!temp)
				return (free(result), NULL);
			new_result = ft_strjoin(result, temp);
			free(result);
			free(temp);
			result = new_result;
			i++;
		}
	}
	return (result);
}

int	create_heredoc_file(t_redir *redir, t_env_list *env)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	redir->filename = generate_heredoc_filename();
	if (!redir->filename)
		return (-1);
	fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (fd_failed(redir, true));
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
		if (!redir->heredoc_quoted)
		{
			expanded_line = expand_heredoc_line(line, env);
			free(line);
			if (!expanded_line)
			{
				close(fd);
				unlink(redir->filename);
				return (fd_failed(redir, false));
			}
			line = expanded_line;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (close(fd), 0);
}

int	setup_heredocs(t_redir *list, t_env_list *env)
{
	while (list)
	{
		if (list->type == TOKEN_HEREDOC && list->delimiter)
		{
			if (create_heredoc_file(list, env) != 0)
			{
				ft_putstr_fd("minishell: heredoc failed\n", 2);
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}

int	setup_all_heredocs(t_ast_node *node, t_env_list *env)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (setup_heredocs(node->redirections, env) != 0)
			return (1);
	}
	if (node->left)
	{
		if (setup_all_heredocs(node->left, env) != 0)
			return (1);
	}
	if (node->right)
	{
		if (setup_all_heredocs(node->right, env) != 0)
			return (1);
	}
	return (0);
}
