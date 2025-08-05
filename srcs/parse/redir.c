/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:31:05 by jgh               #+#    #+#             */
/*   Updated: 2025/08/05 02:37:40 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	process_heredoc(t_redir *redir)
{
	size_t	len;
	char	*temp;

	len = ft_strlen(redir->delimiter);
	if (len < 2)
		return (1);
	if ((redir->delimiter[0] == '\'' && redir->delimiter[len - 1] == '\'')
		|| (redir->delimiter[0] == '"' && redir->delimiter[len - 1] == '"'))
	{
		temp = ft_strndup(redir->delimiter + 1, len - 2);
		if (!temp)
			return (0);
		free(redir->delimiter);
		redir->delimiter = temp;
		redir->heredoc_quoted = true;
	}
	return (1);
}

static int	process_regular(t_redir *redir, t_env_list *env)
{
	char	*expanded;

	expanded = expand_token_value(redir->filename, env);
	if (!expanded)
		return (0);
	free(redir->filename);
	redir->filename = expanded;
	return (1);
}

static int	process_redirections(t_redir *redir,
	t_redir **head, t_env_list *env)
{
	int	success;

	if (redir->is_heredoc)
		success = process_heredoc(redir);
	else
		success = process_regular(redir, env);
	if (!success)
	{
		free_single_redir(redir);
		free_redir_list(*head);
		return (0);
	}
	return (1);
}

static int	handle_redirection(t_token **cur, t_redir **head, t_redir **tail,
	t_env_list *env)
{
	t_redir	*redir;

	if (!(*cur)->next || (*cur)->next->type != TOKEN_WORD)
	{
		free_redir_list(*head);
		return (0);
	}
	redir = new_redir(*cur, (*cur)->next);
	if (!redir)
	{
		free_redir_list(*head);
		return (0);
	}
	if (!process_redirections(redir, head, env))
		return (0);
	append_redir(head, tail, redir);
	return (1);
}

t_redir	*collect_redirections(t_token *start, t_token *end, t_env_list *env)
{
	t_token	*cur;
	t_redir	*head;
	t_redir	*tail;

	if (!start || !end)
		return (NULL);
	head = NULL;
	tail = NULL;
	cur = start;
	while (cur && cur != end->next)
	{
		if (cur->type >= TOKEN_REDIR_IN && cur->type <= TOKEN_HEREDOC)
		{
			if (!handle_redirection(&cur, &head, &tail, env))
				return (NULL);
			cur = cur->next;
		}
		cur = cur->next;
	}
	return (head);
}
