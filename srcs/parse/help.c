/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:30:26 by marvin            #+#    #+#             */
/*   Updated: 2025/07/22 10:30:26 by marvin           ###   ########.fr       */
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

void	read_word(const char *line, size_t *i, t_token **tokens)
{
	char	*word;
	char	c;

	word = NULL;
	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]))
	{
		c = line[*i];
		if (c == '\'')
		{
			(*i)++;
			while (line[*i] && line[*i] != '\'')
				word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
			if (line[*i] == '\'')
				(*i)++;
		}
		else if (c == '"')
		{
			(*i)++;
			while (line[*i] && line[*i] != '"')
				word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
			if (line[*i] == '"')
				(*i)++;
		}
		else
			word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
	}
	if (word)
		add_token(tokens, new_token(TOKEN_WORD, word));
}
