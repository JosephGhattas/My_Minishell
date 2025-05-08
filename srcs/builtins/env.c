/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:28:57 by jghattas          #+#    #+#             */
/*   Updated: 2025/03/28 12:31:17 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**copy_env(char **envp)
{
	int		i;
    int		count;
	char	**env;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(char *) * (count + 1));
    while(i < count)
	{
		env[i] = strdup(envp[i]);			//use my own
		i++;
	}
    env[i] = NULL;
    return (env);
}


int	my_env(int argc, char **argv, char **my_env)
{
	int	i;

	i = 0;
    if (argc > 1)
	{
		printf("env: too many arguments\n");
        return (1);
    }

    while (my_env[i])
    {
		printf("%s\n", my_env[i]);
		i++;
	}
    return (0);
}

