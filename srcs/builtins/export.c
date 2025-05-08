/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:31:14 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 13:38:46 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	my_export(int argc, char **argv, char ***envp)
{
	int		i;
	int		j;
	int		found;
	int		count;
	char	*eq;
	char	**new_env;

	i = 1;
	if (argc < 2)
		return (0);

	while (i < argc)
	{
		eq = ft_strchr(argv[i], '=');  //use my own
        if (!eq)
		{
			printf("export: '%s' not a valid format (must be NAME=VAL)\n", argv[i]);
            continue;
        }
		found = 0;
		j = 0;
        while ((*envp)[j])
		{
            if (strncmp((*envp)[j], argv[i], eq - argv[i]) == 0 &&    // fix strncmp in libft and add strcmp ;;)
                (*envp)[j][eq - argv[i]] == '=')
			{
				free((*envp)[j]);
                (*envp)[j] = ft_strdup(argv[i]);
                found = 1;
                break;
            }
			j++;
        }
        if (!found)
		{
			count = 0;
            while ((*envp)[count])
                count++;

            new_env = malloc(sizeof(char *) * (count + 2));
            j = 0;
			while (j < count)
			{
				new_env[j] = (*envp)[j];
				j++;
			}
			new_env[count] = ft_strdup(argv[i]);
            new_env[count + 1] = NULL;

            free(*envp);
            *envp = new_env;
        }
		i++;
    }

    return (0);
}