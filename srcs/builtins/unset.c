/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:28:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/03/28 12:31:08 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	my_unset(int argc, char **argv, char ***envp)
{
    int		i;
	int		j;
	int		len;
    char	*name;
	int		k;

    i = 1;
    if (argc < 2)
        return (0);

    while (i < argc)
    {
        name = argv[i];
        len = strlen(name); //use my own

        while ((*envp)[j])
		{
            if (strncmp((*envp)[j], name, len) == 0 && (*envp)[j][len] == '=')
			{
				free((*envp)[j]);
				k = j;
                while ((*envp)[k])
				{
					(*envp)[k] = (*envp)[k + 1];
					k++;
				}
                j--; // recheck same index
            }
			j++;
        }
        i++;
    }
    return 0;
}

