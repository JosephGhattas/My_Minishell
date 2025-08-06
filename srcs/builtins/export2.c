/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:27:32 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/06 09:04:04 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env_list	*find_env_var_export(t_env_list *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->type, (char *)key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	append_env_node_export(t_env_list **env, t_env_list *new)
{
	t_env_list	*last;

	if (!*env)
	{
		*env = new;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

t_env_list	*create_env_node_export(char *key, char *val, bool equal)
{
	t_env_list	*new;

	new = malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->type = key;
	new->name = val;
	new->equal = equal;
	new->heredoc_filename = NULL;
	new->exit_status = 0;
	new->shell_pwd = NULL;
	new->shell_oldpwd = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	handle_no_equal(t_env_list **env, const char *arg)
{
	t_env_list	*existing;

	existing = find_env_var_export(*env, arg);
	if (existing)
		return (0);
	else
		append_env_node_export(env,
			create_env_node_export(ft_strdup((char *)arg),
				NULL, false));
	return (0);
}
