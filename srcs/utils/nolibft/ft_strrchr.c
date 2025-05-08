/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:20:58 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/20 04:05:19 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*pos;

	pos = (NULL);
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			pos = s;
		}
		s++;
	}
	if (*s == (char)c)
	{
		pos = s;
	}
	return ((char *)pos);
}
