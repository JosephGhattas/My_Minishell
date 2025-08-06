/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/06 19:28:33 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_var(const char *input, int *i, t_env_list *env)
{
	int		var_len;
	char	*name;
	char	*val;

	name = get_var_name(input + *i + 1, &var_len);
	if (*name == '?' || *name == '$')
		val = get_special_var(*name, env);
	else
		val = get_env_value_exp(name, env);
	free(name);
	*i += var_len + 1;
	return (val);
}

bool	quoted(const char *input, int	*i, char *quote)
{
	if ((input[*i] == '"' || input[*i] == '\'') && *quote == 0)
	{
		*quote = input[*i];
		(*i)++;
		return (true);
	}
	else if ((input[*i] == *quote))
	{
		*quote = 0;
		(*i)++;
		return (true);
	}
	return (false);
}

char	*expand_token_value(const char *input, t_env_list *env)
{
	char	*result;
	char	quote;
	int		i;

	result = ft_strdup("");
	quote = 0;
	i = 0;
	while (input[i])
	{
		if (quoted(input, &i, &quote))
		{
		}
		else if (input[i] == '$' && quote != '\''
			&& input[i + 1] != '\0' && input[i + 1] != '"'
			&& input[i + 1] != ' ')
		{
			result = ft_strjoin_free(result, expand_var(input, &i, env));
		}
		else
		{
			result = ft_strjoin_free(result, ft_char_to_str(input[i]));
			i++;
		}
	}
	return (result);
}
