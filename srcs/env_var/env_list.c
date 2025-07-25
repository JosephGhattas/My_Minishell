/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:39:07 by jgh               #+#    #+#             */
/*   Updated: 2025/07/25 13:04:19 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env_list	*create_env_node(char *env)
{
	t_env_list	*env_list;

	env_list = malloc(sizeof(t_env_list));
	if (!env_list)
		return (NULL);
	env_list->next = NULL;
	env_list->prev = NULL;
	env_list->heredoc_filename = NULL;
	env_list->shell_pwd = NULL;
	env_list->shell_oldpwd = NULL;
	env_list->name = extract_value_part(env);
	env_list->type = extract_key_part(env);
	env_list->equal = equal(env);
	env_list->exit_status = 0;
	return (env_list);
}

void	append_env_node(t_env_list **current, t_env_list **new_node)
{
	(*current)->next = (*new_node);
	(*new_node)->prev = (*current);
	(*current) = (*new_node);
}

t_env_list	*generate_env_list(char **env)
{
	int			i;
	t_env_list	*head;
	t_env_list	*current;
	t_env_list	*new_node;

	i = -1;
	new_node = NULL;
	head = NULL;
	current = NULL;
	while (env[++i])
	{
		new_node = create_env_node(env[i]);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			current = head;
		}
		else
			append_env_node(&current, &new_node);
	}
	return (head);
}

t_env_list	*create_default_env(void)
{
	char	**vars;
	char	*cwd;

	vars = malloc(sizeof(char *) * 4);
	if (!vars)
		return (NULL);
	cwd = NULL;
	cwd = getcwd(cwd, 0);
	vars[0] = ft_strjoin("PWD=", cwd);
	vars[1] = ft_strdup("SHLVL=1");
	vars[2] = ft_strdup("_=/usr/bin/env");
	vars[3] = NULL;
	free(cwd);
	return (generate_env_list(vars));
}

void	cheaking_env(t_env_list **env_list, char **env)
{
	if (env[0] == NULL)
		(*env_list) = create_default_env();
	else
		(*env_list) = generate_env_list(env);
	(*env_list) = add_shell_level((*env_list));
}
