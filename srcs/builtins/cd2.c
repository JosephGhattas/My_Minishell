/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:39:46 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 13:36:28 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

void	append_env_node_cd(t_env_list **head, t_env_list **new_node)
{
	t_env_list	*last;

	if (!head || !new_node || !*new_node)
		return ;
	if (!*head)
	{
		*head = *new_node;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = *new_node;
	(*new_node)->prev = last;
}

void	update_env_var(t_env_list **env, char *key, char *value)
{
	t_env_list	*node;
	char		*joined;

	if (!env || !key || !value)
		return ;
	node = find_env_node(*env, key);
	if (node)
	{
		free(node->name);
		node->name = ft_strdup(value);
		node->equal = true;
	}
	else
	{
		joined = ft_strjoin3(key, "=", value);
		if (!joined)
			return ;
		node = create_env_node(joined);
		free(joined);
		if (!node)
			return ;
		append_env_node(env, &node);
	}
}

char	*get_env_value(t_env_list *env, char *key)
{
	t_env_list	*node;

	node = find_env_node(env, key);
	if (node && node->name)
		return (node->name);
	return (NULL);
}
