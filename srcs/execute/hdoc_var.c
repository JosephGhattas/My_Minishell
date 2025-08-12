/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:53:42 by jgh               #+#    #+#             */
/*   Updated: 2025/08/12 12:39:46 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*handle_escape(char *result,
		const char *input, int *i, bool *escape_next)
{
	result = append_char(result, input[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	*escape_next = false;
	return (result);
}

static char	*handle_backslash(char *result,
		const char *input, int *i, bool *escape_next)
{
	if (input[*i + 1] == '\n')
		*i += 2;
	else if (input[*i + 1] != '\0')
	{
		(*i)++;
		*escape_next = true;
	}
	else
	{
		result = append_char(result, input[(*i)++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

static char	*handle_dollar(char *result,
		const char *input, int *i, t_env_list *env)
{
	return (append_var(result, input, i, env));
}

static char	*handle_default(char *result, const char *input, int *i)
{
	result = append_char(result, input[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

char	*expand_heredoc_line(const char *input, t_env_list *env)
{
	char	*result;
	int		i;
	bool	escape_next;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	escape_next = false;
	while (input[i])
	{
		if (escape_next)
			result = handle_escape(result, input, &i, &escape_next);
		else if (input[i] == '\\')
			result = handle_backslash(result, input, &i, &escape_next);
		else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
			result = handle_dollar(result, input, &i, env);
		else
			result = handle_default(result, input, &i);
		if (!result)
			return (NULL);
	}
	return (result);
}
