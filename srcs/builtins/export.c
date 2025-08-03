/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:31:14 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/01 15:11:00 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_sorted_env(t_env_list *env)
{
	t_env_list	**arr;
	t_env_list	*tmp;
	int			count;
	int			i;

	count = 0;
	tmp = env;
	while (tmp && ++count)
		tmp = tmp->next;
	arr = malloc(count * sizeof(t_env_list *));
	if (!arr)
		return ;
	tmp = env;
	i = 0;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, count);
	i = -1;
	while (++i < count)
		print_one_export(arr[i]);
	free(arr);
}

int	process_arg(t_env_list **env, char *arg)
{
	char	*eq_pos;
	char	*key;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		key = ft_strndup(arg, eq_pos - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		printf("export: `%s': not a valid identifier\n", arg);
		free(key);
		return (0);
	}
	free(key);
	return (update_or_add_env_export(env, arg));
}

int	my_export(int argc, char **argv, t_env_list **env)
{
	int		i;
	int		status;

	if (argc == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 0;
	status = 0;
	while (++i < argc)
	{
		if (process_arg(env, argv[i]) != 0)
			status = 1;
	}
	return (status);
}
