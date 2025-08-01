/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:28:57 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:44:18 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	my_env(int argc, char **argv, t_env_list *env)
{
	t_env_list	*cur;

	(void)argv;
	if (argc > 1)
	{
		printf("env: too many arguments\n");
		return (1);
	}
	cur = env;
	while (cur)
	{
		if (cur->equal)
			printf("%s=%s\n", cur->type, cur->name);
		cur = cur->next;
	}
	return (0);
}

