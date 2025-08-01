/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:03:31 by jgh               #+#    #+#             */
/*   Updated: 2025/08/01 11:55:34 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	**collect_args(t_token *start, t_token *end, int *argc, t_env_list *env)
{
	char	**args;
	t_token	*cur;
	int		i;

	if (!start || !end || !argc)
		return (NULL);
	*argc = count_args(start, end);
	if (*argc < 0)
		return (NULL);
	args = malloc(sizeof(char *) * (*argc + 1));
	if (!args)
		return (NULL);
	cur = start;
	i = 0;
	while (cur && cur != end->next)
	{
		if (cur->type == TOKEN_WORD)
		{
			args[i] = expand_token_value(cur->value, env);
			if (!args[i])
				return (free_args(args, i), NULL);
			i++;
		}
		cur = cur->next;
	}
	args[i] = NULL;
	return (args);
}

static void	append_redir(t_redir **head, t_redir **tail, t_redir *new_redir)
{
	if (!*head)
		*head = new_redir;
	else
		(*tail)->next = new_redir;
	*tail = new_redir;
}

t_redir	*collect_redirections(t_token *start, t_token *end, t_env_list *env)
{
	t_token	*cur;
	t_redir	*head;
	t_redir	*tail;
	t_redir	*redir;

	if (!start || !end)
		return (NULL);
	head = NULL;
	tail = NULL;
	cur = start;
	while (cur && cur != end->next)
	{
		if (cur->type >= TOKEN_REDIR_IN && cur->type <= TOKEN_HEREDOC)
		{
			if (!cur->next || cur->next->type != TOKEN_WORD)
				return (NULL);
			redir = new_redir(cur, cur->next);
			if(!redir)
				return (NULL);
			if (redir)
			{
				if (redir->is_heredoc)
				{
					// Heredoc delimiter handling
					size_t len = ft_strlen(redir->delimiter);
					if (len >= 2 && ((redir->delimiter[0] == '\'' && 
						redir->delimiter[len-1] == '\'') ||
						(redir->delimiter[0] == '"' && 
						redir->delimiter[len-1] == '"')))
					{
						char *temp = ft_strndup(redir->delimiter+1, len-2);
						free(redir->delimiter);
						redir->delimiter = temp;
						redir->heredoc_quoted = true;
					}
				}
				else if (redir->filename)
				{
					char *expanded = expand_token_value(redir->filename, env);
					free(redir->filename);
					redir->filename = expanded;
				}
			}
			append_redir(&head, &tail, redir);
			cur = cur->next;
		}
		cur = cur->next;
	}
	return (head);
}

// void	print_indent(int depth)
// {
// 	while (depth-- > 0)
// 		printf("    ");
// }

// void	print_redirections(t_redir *redir, int depth)
// {
// 	while (redir)
// 	{
// 		print_indent(depth);
// 		printf("REDIR: ");
// 		if (redir->type == TOKEN_REDIR_IN)
// 			printf("< ");
// 		else if (redir->type == TOKEN_REDIR_OUT)
// 			printf("> ");
// 		else if (redir->type == TOKEN_REDIR_APPEND)
// 			printf(">> ");
// 		else if (redir->type == TOKEN_HEREDOC)
// 			printf("<< ");
// 		if (redir->is_heredoc && redir->delimiter)
// 			printf("%s (delimiter)\n", redir->delimiter);
// 		else if (redir->filename)
// 			printf("%s\n", redir->filename);
// 		else
// 			printf("(null)\n");
// 		redir = redir->next;
// 	}
// }

// void	print_args(char **args, int argc, int depth)
// {
// 	for (int i = 0; i < argc; i++)
// 	{
// 		print_indent(depth);
// 		printf("ARG[%d]: %s\n", i, args[i]);
// 	}
// }

// void	print_ast(t_ast_node *node, int depth)
// {
// 	if (!node)
// 	{
// 		print_indent(depth);
// 		printf("(null)\n");
// 		return;
// 	}

// 	print_indent(depth);
// 	if (node->type == NODE_PIPE)
// 	{
// 		printf("PIPE\n");

// 		print_indent(depth);
// 		printf("Left:\n");
// 		print_ast(node->left, depth + 1);

// 		print_indent(depth);
// 		printf("Right:\n");
// 		print_ast(node->right, depth + 1);
// 	}
// 	else if (node->type == NODE_COMMAND)
// 	{
// 		printf("COMMAND\n");
// 		if (node->args)
// 			print_args(node->args, node->argc, depth + 1);
// 		if (node->redirections)
// 			print_redirections(node->redirections, depth + 1);
// 	}
// }
