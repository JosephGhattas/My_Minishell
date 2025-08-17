/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:29:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:45:55 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_strtoll_core(const char *p, long long *num, int sign)
{
	int	digit;

	while (*p)
	{
		if (*p < '0' || *p > '9')
			return (0);
		digit = *p - '0';
		if (sign == 1)
		{
			if (*num > (LLONG_MAX - digit) / 10)
				return (0);
			*num = *num * 10 + digit;
		}
		else
		{
			if (*num < LLONG_MIN / 10)
				return (0);
			*num *= 10;
			if (*num < LLONG_MIN + digit)
				return (0);
			*num -= digit;
		}
		p++;
	}
	return (1);
}

int	ft_strtoll(const char *str, long long *result)
{
	long long	num;
	int			sign;
	const char	*p;

	num = 0;
	sign = 1;
	p = str;
	if (*p == '-' || *p == '+')
	{
		if (*p == '-')
			sign = -1;
		p++;
	}
	if (*p == '\0')
		return (0);
	if (!ft_strtoll_core(p, &num, sign))
		return (0);
	*result = num;
	return (1);
}

int	too_many_args(void)
{
	write(2, "minishell: exit: too many arguments\n", 36);
	return (1);
}

int	my_exit(t_ast_node *cmd, int argc, char **argv, t_env_list **env)
{
	int			code;
	long long	num;

	printf("exit\n");
	(void)cmd;
	if (argc == 1)
		code = get_exit_status(*env);
	else if (!ft_strtoll(argv[1], &num))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		frees(cmd, *env);
		exit(2);
	}
	else if (argc > 2)
		return (too_many_args());
	else
		code = (unsigned char)num;
	frees(cmd, *env);
	exit(code);
}
