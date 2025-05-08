/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:50:57 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/25 01:28:48 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

static int	ft_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len ++;
	return (len);
}

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;
	size_t	saved_len;

	src_len = ft_len(src);
	dest_len = ft_len(dest);
	saved_len = ft_len(dest);
	if (size <= dest_len)
		return (src_len + size);
	i = 0;
	while (src[i] != '\0' && dest_len < size - 1)
	{
		dest[dest_len] = src[i];
		dest_len ++;
		i ++;
	}
	dest[dest_len] = '\0';
	return (src_len + saved_len);
}
