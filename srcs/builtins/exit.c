/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:29:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 12:21:01 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_numeric(const char *s)
{
	int	i;

	if (!s)
		return (0);
    i = 0;
    if (s[0] == '-' || s[0] == '+')
		i++;
    while (s[i])
	{
        if (!isdigit(s[i]))
			return (0);
        i++;
    }
    return (1);
}

int	my_exit(int argc, char **argv)
{
	int	code;

	printf("exit\n");

    if (argc == 1)
        exit(0);

    if (!is_numeric(argv[1]))
	{
        printf("exit: %s: numeric argument required\n", argv[1]);
        exit(255);
    }
    if (argc > 2)
	{
        printf("exit: too many arguments\n");
        return (1);
    }

	code = ft_atoi(argv[1]);
    exit(code);
}
