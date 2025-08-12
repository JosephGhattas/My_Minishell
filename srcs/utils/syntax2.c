/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:19:00 by jgh               #+#    #+#             */
/*   Updated: 2025/08/12 11:19:37 by jgh              ###   ########.fr       */
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
