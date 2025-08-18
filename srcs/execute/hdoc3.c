/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:21:47 by jgh               #+#    #+#             */
/*   Updated: 2025/08/18 09:22:03 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

#include "../../minishell.h"

static char	*str_append_char(char *s, char c)
{
	char	*new;
	int		len;

	if (!s)
	{
		new = malloc(2);
		if (!new)
			return (NULL);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	len = ft_strlen(s);
	new = malloc(len + 2);
	if (!new)
	{
		free(s);
		return (NULL);
	}
	ft_strlcpy(new, s, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	free(s);
	return (new);
}

char	*read_heredoc_line(void)
{
	char	*line;
	char	c;
	int		n;

	line = NULL;
	while (1)
	{
		n = read(0, &c, 1);
		if (n <= 0)
		{
			if (n == 0)
				break ;
			if (n < 0 && errno == EINTR && g_sig == SIGINT)
				break ;
			continue;
		}
		if (c == '\n')
			break ;
		line = str_append_char(line, c);
		if (!line)
			return (NULL);
	}
	if (g_sig == SIGINT)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
