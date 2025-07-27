/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:47:49 by jgh               #+#    #+#             */
/*   Updated: 2025/07/27 00:25:31 by jgh              ###   ########.fr       */
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

t_token	*find_last_token_of_type(t_token *tail, t_token_type type)
{
	while (tail)
	{
		if (tail->type == type)
			return (tail);
		tail = tail->prev;
	}
	return (NULL);
}

t_redir	*new_redir(t_token *token, t_token *next)
{
	t_redir	*redir;

	if	(!token || !next)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	redir->is_heredoc = (token->type == TOKEN_HEREDOC);
	redir->filename = NULL;
	redir->delimiter = NULL;
	redir->next = NULL;
	if (redir->is_heredoc && next)
		redir->delimiter = ft_strdup(next->value);
	else if (next)
		redir->filename = ft_strdup(next->value);
	return (redir);
}

int	count_args(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end->next)
	{
		if (start->type == TOKEN_WORD)
			count++;
		start = start->next;
	}
	return (count);
}
