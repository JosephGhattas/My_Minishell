/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 12:20:41 by jgh               #+#    #+#             */
/*   Updated: 2025/08/02 16:39:48 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

bool	print_redir_error(char c)
{
	char	buf[64];
	int		len;

	if (c == '\0')
	{
		ft_putstr_fd(
		"CJminishell: syntax error near unexpected token `newline'\n", 2);
	}
	else
	{
		len = snprintf(buf, sizeof(buf),
			"CJminishell: syntax error near unexpected token `%c'\n", c);
		write(2, buf, len);
	}
	return (true);
}

/*
** Detect invalid metacharacters: \;&(){}[]`
*/
bool	detect_invalid_metachar(const char *s)
{
	const char	*bad;
	char		q;
	int			i;

	bad = "\\;&(){}[]`";
	q   = 0;
	i   = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (q == 0)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		else if (q == 0 && ft_strchr(bad, s[i]))
		{
			ft_putstr_fd("CJminishell: syntax error near unexpected token `", 2);
			write(2, &s[i], 1);
			ft_putstr_fd("'\n", 2);
			return (true);
		}
		i++;
	}
	return (false);
}

/*
** Consume digits, << or >>, check for triple, then ensure valid target.
*/
bool	process_redirection(const char *s, int *i)
{
	char	op;
	char	next;

	while (ft_isdigit((unsigned char)s[*i]))
		(*i)++;
	op = s[(*i)++];
	if (s[*i] == op)
	{
		(*i)++;
		if (s[*i] == op)
			return (print_redir_error(op));
	}
	while (s[*i] && ft_isspace((unsigned char)s[*i]))
		(*i)++;
	next = s[*i];
	if (next == '\0' || next == '|' || next == '<' || next == '>')
		return (print_redir_error(next));
	return (false);
}

/*
** Detect <, >, <<, >>, digit+<>, and missing or invalid targets.
*/
bool	detect_redir_errors(const char *s)
{
	char	q;
	int		i;

	q = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (q == 0)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		else if (q == 0 && (s[i] == '<' || s[i] == '>'
			|| (ft_isdigit((unsigned char)s[i])
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
