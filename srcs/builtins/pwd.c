/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 22:59:04 by jgh               #+#    #+#             */
/*   Updated: 2025/08/03 22:31:29 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*safe_getcwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}

t_env_list	*find_env_node(t_env_list *env, char *key)
{
	t_env_list	*curr;

	if (!env || !key)
		return (NULL);
	curr = env;
	while (curr)
	{
		if (curr->type && ft_strcmp(curr->type, key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

int	my_pwd(t_env_list *env)
{
	t_env_list	*pwd_node;
	char		*cwd;

	pwd_node = find_env_node(env, "PWD");
	if (pwd_node && pwd_node->name)
	{
		printf("%s\n", pwd_node->name);
		return (0);
	}
	else
	{
		cwd = safe_getcwd();
		if (!*cwd)
		{
			perror("pwd");
			return (1);
		}
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
}
