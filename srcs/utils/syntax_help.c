/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:14:49 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/12 15:14:51 by jghattas         ###   ########.fr       */
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

static bool	check_redirection_at_pos(const char *s, int *i)
{
	if (s[*i] == '<' || s[*i] == '>'
		|| (ft_isdigit((unsigned char)s[*i])
			&& s[*i + 1] != '\0' && ft_strchr("<>", s[*i + 1])))
	{
		if (process_redirection(s, i))
			return (true);
	}
	else
		(*i)++;
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
		{
			i++;
			if (s[i] == '\0')
				break ;
		}
		if (q == 0 && check_redirection_at_pos(s, &i))
			return (true);
		else if (q != 0)
			i++;
	}
	return (false);
}
