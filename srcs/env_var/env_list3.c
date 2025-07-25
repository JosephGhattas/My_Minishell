/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:58:44 by jgh               #+#    #+#             */
/*   Updated: 2025/07/25 13:01:59 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*extract_value_part(char *env)
{
	int		i;
	int		value_index;
	char	*value_str;

	i = 0;
	while (env[i] != '=')
		i++;
	value_str = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!value_str)
		return (NULL);
	i++;
	value_index = i;
	i = 0;
	while (env[value_index])
	{
		value_str[i] = env[value_index];
		i++;
		value_index++;
	}
	value_str[i] = '\0';
	return (value_str);
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
