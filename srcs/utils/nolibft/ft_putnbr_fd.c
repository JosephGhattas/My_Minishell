/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 11:54:12 by jghattas          #+#    #+#             */
/*   Updated: 2024/06/20 04:04:16 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	nbr;
	long	nbl;

	nbl = n;
	if (nbl < 0)
	{
		nbl = -nbl;
		write(fd, "-", 1);
	}
	if (nbl >= 10)
	{
		ft_putnbr_fd(nbl / 10, fd);
	}
	nbr = nbl % 10 + '0';
	write(fd, &nbr, 1);
}
