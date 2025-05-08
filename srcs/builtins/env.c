/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:28:57 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 13:36:56 by jghattas         ###   ########.fr       */
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
		env[i] = ft_strdup(envp[i]);
		i++;
	}
    env[i] = NULL;
    return (env);
}


int	my_env(int argc, char **argv, char ***my_env)
{
	int	i;

	char **envp = *my_env;
	(void)argv;
	i = 0;
    if (argc > 1)
	{
		printf("env: too many arguments\n");
        return (1);
    }

    while (envp[i])
    {
		printf("%s\n", envp[i]);
		i++;
	}
    return (0);
}

