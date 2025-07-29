/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:18:11 by jgh               #+#    #+#             */
/*   Updated: 2025/07/29 13:20:04 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	env_list_size(t_env_list *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->type && env->name)
			count++;
		env = env->next;
	}
	return (count);
}

char	*join_env_entry(t_env_list *node)
{
	char	*tmp;
	char	*joined;

	if (!node || !node->type || !node->name)
		return (NULL);
	tmp = ft_strjoin(node->type, "=");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, node->name);
	free(tmp);
	return (joined);
}

char	**env_list_to_envp(t_env_list *env)
{
	char		**envp;
	char		*entry;
	int			size;
	int			i;

	size = env_list_size(env);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->type && env->name)
		{
			entry = join_env_entry(env);
			if (!entry)
			{
				while (i--)
					free(envp[i]);
				free(envp);
				return (NULL);
			}
			envp[i++] = entry;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
