/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 00:21:55 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/20 04:03:40 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	get_length(int n)
{
	int	length;

	length = 0;
	if (n < 1)
		length++;
	while (n)
	{
		n /= 10;
		length++;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	int			is_negative;
	int			length;
	char		*str;
	long long	nb;

	is_negative = 0;
	length = get_length(n);
	str = (char *)malloc((length + 1));
	if (str == NULL)
		return (NULL);
	nb = n;
	if (nb < 0)
	{
		is_negative = 1;
		nb *= -1;
	}
	str[length] = '\0';
	while (length--)
	{
		str[length] = (nb % 10) + '0';
		nb /= 10;
	}
	if (is_negative)
		*(str) = '-';
	return (str);
}
