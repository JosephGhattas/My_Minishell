/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:57:55 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/25 01:29:42 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(char *src);

static int	ft_len(char	*src)
{
	int	len;

	len = 0;
	while (src[len] != '\0')
		len ++;
	return (len);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		len;
	int		i;

	if (!src)
		return (NULL);
	len = ft_len(src);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
