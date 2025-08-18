/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 12:28:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:22:18 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_internal_var(const char *name)
{
	if (!name)
		return (0);
	if (name[0] == '?' && name[1] == '\0')
		return (1);
	return (0);
}

static int	is_valid_unset_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	remove_env_var(t_env_list **env, const char *key)
{
	t_env_list	*cur;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->type, (char *)key) == 0)
		{
			if (cur->prev)
				cur->prev->next = cur->next;
			else
				*env = cur->next;
			if (cur->next)
				cur->next->prev = cur->prev;
			free(cur->type);
			free(cur->name);
			free(cur);
			return ;
		}
		cur = cur->next;
	}
}

int	my_unset(int argc, char **argv, t_env_list **env)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (is_internal_var(argv[i]))
		{
			i++;
			continue ;
		}
		if (!is_valid_unset_key(argv[i]))
		{
			printf("unset: `%s': not a valid identifier\n", argv[i]);
			i++;
			continue ;
		}
		remove_env_var(env, argv[i]);
		i++;
	}
	return (0);
}
