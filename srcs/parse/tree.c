/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:03:31 by jgh               #+#    #+#             */
/*   Updated: 2025/08/04 14:35:23 by jgh              ###   ########.fr       */
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

void	append_redir(t_redir **head, t_redir **tail, t_redir *new_redir)
{
	if (!*head)
		*head = new_redir;
	else
		(*tail)->next = new_redir;
	*tail = new_redir;
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

void	free_single_redir(t_redir *redir)
{
	if (!redir)
		return ;
	free(redir->filename);
	free(redir->delimiter);
	free(redir);
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
