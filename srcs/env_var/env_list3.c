/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:15 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/10 18:01:22 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*extract_value_part(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '=')
		i++;
	return (ft_strdup(env + i));
}

char	*extract_key_part(char *env_var)
{
	int		index;
	char	*key_str;

	index = 0;
	while (env_var[index] != '\0' && env_var[index] != '=')
		index++;
	key_str = malloc(sizeof(char) * (index + 1));
	if (!key_str)
		return (NULL);
	index = 0;
	while (env_var[index] != '\0' && env_var[index] != '=')
	{
		key_str[index] = env_var[index];
		index++;
	}
	key_str[index] = '\0';
	return (key_str);
}

bool	equal(char *env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

static void	free_vars(char **var, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(var[j]);
		j++;
	}
	free(var);
	memory_error();
}

static char	**init_def_var(void)
{
	char		*cwd;
	char		**vars;

	vars = malloc(sizeof(char *) * 4);
	if (!vars)
		memory_error();
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		cwd = ft_strdup("/");
		if (!cwd)
			free_vars(vars, 0);
	}
	vars[0] = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!vars[0])
		free_vars(vars, 0);
	vars[1] = ft_strdup("SHLVL=1");
	if (!vars[1])
		free_vars(vars, 1);
	vars[2] = ft_strdup("_=/usr/bin/env");
	if (!vars[2])
		free_vars(vars, 2);
	return (vars);
}

t_env_list	*create_default_env(void)
{
	char		**vars;
	t_env_list	*list;

	vars = init_def_var();
	vars[3] = NULL;
	list = generate_env_list(vars);
	free_array(vars);
	if (!list)
		memory_error();
	return (list);
}
