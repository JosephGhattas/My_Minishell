/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:14:30 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/12 15:14:31 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_redir	*alloc_redir(t_token *token)
{
	t_redir	*redir;

	if (!token)
		return (NULL);
	redir = malloc (sizeof (*redir));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	redir->is_heredoc = (token->type == TOKEN_HEREDOC);
	redir->filename = NULL;
	redir->delimiter = NULL;
	redir->next = NULL;
	redir->heredoc_quoted = false;
	return (redir);
}

static int	set_redir_target(t_redir *redir, t_token *next)
{
	char	*dup;

	if (!redir)
		return (-1);
	if (!next)
		return (0);
	dup = ft_strdup(next->value);
	if (!dup)
		return (-1);
	if (redir->is_heredoc)
		redir->delimiter = dup;
	else
		redir->filename = dup;
	return (0);
}

t_redir	*new_redir(t_token *token, t_token *next)
{
	t_redir	*redir;

	if (!token || !next)
		return (NULL);
	redir = alloc_redir(token);
	if (!redir)
		return (NULL);
	if (set_redir_target(redir, next) != 0)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}
