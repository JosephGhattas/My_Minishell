/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:47:20 by jgh               #+#    #+#             */
/*   Updated: 2025/07/25 13:47:20 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_list_full(t_env_list *env)
{
	t_env_list	*tmp;

	if (env == NULL)
		return ;
	while (env)
	{
		tmp = env->next;
		if (env->name != NULL)
			free(env->name);
		if (env->type != NULL)
			free(env->type);
		if (env->heredoc_filename != NULL)
			free(env->heredoc_filename);
		if (env->shell_pwd != NULL)
			free(env->shell_pwd);
		if (env->shell_oldpwd != NULL)
			free(env->shell_oldpwd);
		free(env);
		env = tmp;
	}
}

void	free_tokens(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		if (tok->value)
			free(tok->value);
		free(tok);
		tok = tmp;
	}
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_redir_list(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->filename)
		{
			unlink(head->filename);
			free(head->filename);
		}
		if (head->delimiter)
			free(head->delimiter);
		free(head);
		head = tmp;
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		free_array(node->args);
		// free_redir_list(node->redirections);
	}
	else
	{
		free_ast(node->left);
		free_ast(node->right);
	}
	free(node);
}
