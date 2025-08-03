/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:06:08 by jgh               #+#    #+#             */
/*   Updated: 2025/08/03 12:46:39 by jgh              ###   ########.fr       */
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

char	*ft_char_to_str(char c)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
	{
		res = ft_strdup(s2);
		return (free(s2), res);
	}
	else if (!s2)
	{
		res = ft_strdup(s1);
		return (free(s1), res);
	}
	res = ft_strjoin(s1, s2);
	if (!res)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (res);
}

char	*ft_strndup(char *src, unsigned int n)
{
	unsigned int	len;
	unsigned int	i;
	char			*dest;

	if (!src)
		return (NULL);
	len = 0;
	while (len < n && src[len] != '\0')
		len++;
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
