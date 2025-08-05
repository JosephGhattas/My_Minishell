/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:08 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/05 15:29:08 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	read_operator(const char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		add_token(tokens, new_token(TOKEN_REDIR_APPEND, ft_strdup(">>")));
		*i += 2;
	}
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_token(tokens, new_token(TOKEN_HEREDOC, ft_strdup("<<")));
		*i += 2;
	}
	else if (line[*i] == '>')
	{
		add_token(tokens, new_token(TOKEN_REDIR_OUT, ft_strdup(">")));
		(*i)++;
	}
	else if (line[*i] == '<')
	{
		add_token(tokens, new_token(TOKEN_REDIR_IN, ft_strdup("<")));
		(*i)++;
	}
	else if (line[*i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, ft_strdup("|")));
		(*i)++;
	}
}

char	*quoted_word(const char *line, size_t *i, char *word)
{
	char	c;

	c = line[*i];
	word = ft_strjoin_free(word, ft_char_to_str(c));
	(*i)++;
	while (line[*i] && line[*i] != c)
	{
		word = ft_strjoin_free(word, ft_char_to_str(line[*i]));
		(*i)++;
	}
	if (line[*i] == c)
	{
		word = ft_strjoin_free(word, ft_char_to_str(line[*i]));
		(*i)++;
	}
	return (word);
}

void	read_word(const char *line, size_t *i, t_token **tokens)
{
	char	*word;
	char	c;

	word = NULL;
	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]))
	{
		c = line[*i];
		if (c == '\'' || c == '"')
			word = quoted_word(line, i, word);
		else
			word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
	}
	if (word)
		add_token(tokens, new_token(TOKEN_WORD, word));
}

void	append_redir(t_redir **head, t_redir **tail, t_redir *new_redir)
{
	if (!*head)
		*head = new_redir;
	else
		(*tail)->next = new_redir;
	*tail = new_redir;
}

void	free_single_redir(t_redir *redir)
{
	if (!redir)
		return ;
	free(redir->filename);
	free(redir->delimiter);
	free(redir);
}
