/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:20:41 by jgh               #+#    #+#             */
/*   Updated: 2025/08/05 03:23:04 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	update_quote(char current_quote, char c)
{
	if ((c == '"' || c == '\''))
	{
		if (current_quote == 0)
			return (c);
		else if (current_quote == c)
			return (0);
	}
	return (current_quote);
}

bool	detect_consecutive_pipes(const char *s)
{
	bool	last_pipe;
	char	quote;
	int		i;

	last_pipe = false;
	quote = 0;
	i = 0;
	while (s[i])
	{
		quote = update_quote(quote, s[i]);
		if (s[i] == '|' && quote == 0)
		{
			if (last_pipe)
			{
				ft_putstr_fd(
					"minishell: syntax error near unexpected token `|'\n", 2);
				return (true);
			}
			last_pipe = true;
		}
		else if (!ft_isspace((unsigned char)s[i]) && quote == 0)
			last_pipe = false;
		i++;
	}
	return (false);
}

bool	leading_pipe(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (false);
	while (ft_isspace((unsigned char)s[i]))
		i++;
	if (s[i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (true);
	}
	return (false);
}

bool	trailing_pipe(const char *s)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		quote = update_quote(quote, s[i]);
		i++;
	}
	while (i > 0 && ft_isspace((unsigned char)s[i - 1]))
		i--;
	if (i > 0 && s[i - 1] == '|' && quote == 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (true);
	}
	if (quote != 0)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (true);
	}
	return (false);
}

bool	detect_syntax_errors(const char *input)
{
	if (!input || is_only_whitespace(input))
		return (false);
	if (trailing_pipe(input))
		return (true);
	if (leading_pipe(input))
		return (true);
	if (detect_invalid_metachar(input))
		return (true);
	if (detect_redir_errors(input))
		return (true);
	if (detect_consecutive_pipes(input))
		return (true);
	return (false);
}
