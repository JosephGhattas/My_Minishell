/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:02:01 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/25 01:20:39 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*space;
	size_t		i;
	size_t		s_len;

	s_len = ft_strlen((char *)s);
	if (!s)
		return (NULL);
	if (start > s_len)
		len = 0;
	else if (len > s_len - start)
		len = s_len - start;
	space = (char *)malloc(sizeof (char) * (len + 1));
	if (!space)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		space[i] = s[start + i];
		i++;
	}
	space[i] = '\0';
	return (space);
}
