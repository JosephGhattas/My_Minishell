/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:45 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/05 15:29:46 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_special_var(char c, t_env_list *env)
{
	t_env_list	*node;

	if (c == '?')
	{
		node = find_env_node(env, "?");
		if (node)
			return (ft_strdup(node->name));
		else
			return (ft_strdup("0"));
	}
	if (c == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}

char	*get_var_name(const char *s, int *len)
{
	int	i;

	i = 0;
	if (s[0] == '?' || s[0] == '$')
	{
		*len = 1;
		return (ft_strndup ((char *)s, 1));
	}
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	*len = i;
	return (ft_strndup((char *)s, i));
}

char	*get_env_value_exp(const char *key, t_env_list *env)
{
	t_env_list	*node;

	node = find_env_node(env, (char *)key);
	if (node && node->equal)
		return (ft_strdup(node->name));
	return (ft_strdup(""));
}

void	update_exit_status(t_env_list **env, int status)
{
	char	*s;

	s = ft_itoa(status);
	update_env_var(env, "?", s);
	free(s);
}

int	get_exit_status(t_env_list *env)
{
	t_env_list	*node;

	node = find_env_node(env, "?");
	if (!node)
		return (0);
	return (ft_atoi(node->name));
}
