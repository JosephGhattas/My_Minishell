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
	int		j;
	char	*dest;

	dest = (char *)malloc((ft_len(src) + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	j = 0;
	while (src[j] != '\0')
	{
		dest[j] = src[j];
		j ++;
	}
	dest[j] = '\0';
	return (dest);
}
