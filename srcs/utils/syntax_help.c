/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:47:11 by jgh               #+#    #+#             */
/*   Updated: 2025/08/05 03:23:16 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	print_redir_error(char c)
{
	if (c == '\0')
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n", 2);
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		write(2, &c, 1);
		ft_putstr_fd("'\n", 2);
	}
	return (true);
}

static bool	process_redirection(const char *s, int *i)
{
	char	op;
	int		len;

	len = ft_strlen(s);
	while (*i < len && ft_isdigit((unsigned char)s[*i]))
		(*i)++;
	if (*i >= len)
		return (print_redir_error('\0'));
	op = s[(*i)++];
	if (*i < len && s[*i] == op)
	{
		(*i)++;
		if (*i < len && s[*i] == op)
			return (print_redir_error(op));
	}
	while (*i < len && s[*i] && ft_isspace((unsigned char)s[*i]))
		(*i)++;
	if (*i >= len)
		return (print_redir_error('\0'));
	if (s[*i] == '\0' || s[*i] == '|' || s[*i] == '<' || s[*i] == '>')
		return (print_redir_error(s[*i]));
	return (false);
}

bool	detect_redir_errors(const char *s)
{
	char	q;
	int		i;
	int		temp;

	q = 0;
	i = 0;
	while (s[i])
	{
		temp = q;
		q = update_quote(q, s[i]);
		if (q != temp)
			i++;
		if (q == 0 && (s[i] == '<' || s[i] == '>'
				|| (ft_isdigit((unsigned char)s[i])
					&& s[i + 1] != '\0'
					&& ft_strchr("<>", s[i + 1]))))
		{
			if (process_redirection(s, &i))
				return (true);
		}
		else
			i++;
	}
	return (false);
}

bool	is_only_whitespace(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isspace((unsigned char)s[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	detect_invalid_metachar(const char *s)
{
	const char	*bad = "\\;&(){}[]`";
	char		q;
	int			i;

	q = 0;
	i = 0;
	while (s[i])
	{
		q = update_quote(q, s[i]);
		if (q == 0 && ft_strchr(bad, s[i]))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			write(2, &s[i], 1);
			ft_putstr_fd("'\n", 2);
			return (true);
		}
		i++;
	}
	return (false);
}
