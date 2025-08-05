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

//handle updating the exit code && numeric range form 0 255
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
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	my_exit(int argc, char **argv, t_env_list **env)
{
	int	code;

	printf("exit\n");
	if (argc == 1)
		exit(get_exit_status(*env));
	else if (!is_numeric(argv[1]))
	{
		printf("Minishell: exit: %s: numeric argument required\n", argv[1]);
		code = 2;
	}
	else if (argc > 2)
	{
		printf("Minishell: exit: too many arguments\n");
		return (1);
	}
	else
		code = ft_atoi(argv[1]);
	// exit(code);
	return (code);
}
