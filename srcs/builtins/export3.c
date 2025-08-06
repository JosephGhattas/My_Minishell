/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:27:37 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/06 08:05:08 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_with_equal(t_env_list **env, const char *arg, char *eq)
{
	char		*key;
	char		*val;
	t_env_list	*existing;

	key = ft_substr(arg, 0, eq - arg);
	val = ft_strdup(eq + 1);
	if (!key || !val)
		return (free(key), free(val), 1);
	existing = find_env_var_export(*env, key);
	if (existing)
	{
		if (existing->name)
			free(existing->name);
		existing->name = val;
		existing->equal = true;
		free(key);
	}
	else
		append_env_node_export(env, create_env_node_export(key, val, true));
	return (0);
}

int	update_or_add_env_export(t_env_list **env, const char *arg)
{
	char		*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (handle_no_equal(env, arg));
	else
		return (handle_with_equal(env, arg, eq));
}

void	print_one_export(t_env_list *node)
{
	char	*line;
	char	*tmp;

	if (node->equal)
	{
		line = ft_strjoin("declare -x ", node->type);
		tmp = ft_strjoin(line, "=\"");
		free(line);
		line = ft_strjoin(tmp, node->name);
		free(tmp);
		tmp = ft_strjoin(line, "\"\n");
		free(line);
		ft_putstr_fd(tmp, 1);
		free(tmp);
	}
	else
	{
		line = ft_strjoin("declare -x ", node->type);
		ft_putendl_fd(line, 1);
		free(line);
	}
}

void	sort_env_array(t_env_list **arr, int count)
{
	int			i;
	int			j;
	t_env_list	*swap;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->type, arr[j]->type) > 0)
			{
				swap = arr[i];
				arr[i] = arr[j];
				arr[j] = swap;
			}
			j++;
		}
		i++;
	}
}
