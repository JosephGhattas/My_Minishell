/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:41:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 13:39:07 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	my_echo(int argc, char**argv)
{
	int	no_line;
	int	i;
	int	j;

	no_line = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			j = 1;
			while (argv[i][j] == 'n')
				j++;
			if (argv[i][j] == '\0')
			{
				no_line = 1;
				i++;
			}
			else
				break ;
		}
		else
			break ;
	}
	while (i < argc)
	{
		printf("%s", argv[i]);
		i++;
		if (i < argc)
			printf(" ");
	}
	if (no_line == 0)
		printf("\n");
	return (0);
}
