/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:06 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/05 15:28:07 by jghattas         ###   ########.fr       */
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
	if (!env_list->name)
		return (free(env_list), NULL);
	env_list->type = extract_key_part(env);
	if (!env_list->type)
	{
		free(env_list->name);
		free(env_list);
		return (NULL);
	}
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

static void	free_list(t_env_list *head)
{
	t_env_list	*current;

	while (head)
	{
		current = head->next;
		free(head->name);
		free(head->type);
		free(head);
		head = current;
	}
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
			return (free_list(head), NULL);
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

void	cheaking_env(t_env_list **env_list, char **env)
{
	if (env[0] == NULL)
		(*env_list) = create_default_env();
	else
		(*env_list) = generate_env_list(env);
	if (!(*env_list))
		memory_error();
	(*env_list) = add_shell_level((*env_list));
}
