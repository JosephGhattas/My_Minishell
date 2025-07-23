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

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '\0');
}

char *ft_char_to_str(char c)
{
	char *str = malloc(2);
	if (!str)
		return (NULL); // Defensive
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char *res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}


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
	char	*word = NULL;
	char	c;

	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]))
	{
		c = line[*i];
		if (c == '\'')
		{
			(*i)++;
			while (line[*i] && line[*i] != '\'')
				word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
			if (line[*i] == '\'') (*i)++;
		}
		else if (c == '"')
		{
			(*i)++;
			while (line[*i] && line[*i] != '"')
				word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
			if (line[*i] == '"') (*i)++;
		}
		else
			word = ft_strjoin_free(word, ft_char_to_str(line[(*i)++]));
	}
	if (word)
		add_token(tokens, new_token(TOKEN_WORD, word));
}
