/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:34 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/12 10:52:08 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*find_last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

t_token	*find_first_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->prev)
		token = token->prev;
	return (token);
}

t_token	*find_last_token_of_type(t_token *start, t_token *end,
	t_token_type type)
{
	t_token	*cur;

	cur = end;
	if (!start || !end)
		return (NULL);
	while (cur && cur != start->prev)
	{
		if (cur->type == type)
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

int	count_args(t_token *start, t_token *end)
{
	int		count;
	t_token	*cur;
	t_token	*prev;

	cur = start;
	count = 0;
	prev = NULL;
	while (cur && cur != end->next)
	{
		if (cur->type == TOKEN_WORD)
		{
			if (prev && (prev->type >= TOKEN_REDIR_IN
					&& prev->type <= TOKEN_HEREDOC))
			{
			}
			else
				count++;
		}
		prev = cur;
		cur = cur->next;
	}
	return (count);
}
