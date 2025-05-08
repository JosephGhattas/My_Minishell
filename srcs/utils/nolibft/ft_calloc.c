/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:28:44 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/20 04:03:06 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			totalsize;
	void			*ptr;
	unsigned char	*pt;
	size_t			i;

	totalsize = nmemb * size;
	ptr = (char *)malloc(totalsize);
	if (ptr != NULL)
	{
		pt = (unsigned char *)ptr;
		i = 0;
		while (i < totalsize)
		{
			pt[i] = 0;
			i++;
		}
	}
	return (ptr);
}
