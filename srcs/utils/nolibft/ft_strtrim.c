/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:37:38 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/20 04:23:13 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set);

int	found(char c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static int	ft_strlenght(const char *c)
{
	size_t	len;

	len = 0;
	while (c[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t		i;
	size_t		start;
	size_t		end;
	char		*new;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	end = ft_strlenght(s1);
	while (s1[start] != '\0' && found(s1[start], set))
		start++;
	while (end > start && found(s1[end - 1], set))
		end--;
	new = (char *)malloc(end - start + 1);
	if (new == NULL)
		return (NULL);
	i = 0;
	while (i < (end - start))
	{
		new[i] = s1[start + i];
		i++;
	}
	new[end - start] = '\0';
	return (new);
}
