/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:39 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/06 19:53:26 by jghattas         ###   ########.fr       */
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

bool	should_expand(t_token *cur, t_token *prev)
{
	if (!cur || cur->type != TOKEN_WORD)
		return (false);
	if (prev && prev->type >= TOKEN_REDIR_IN && prev->type <= TOKEN_HEREDOC)
		return (false);
	return (true);
}

static bool	assign_expanded_token(char **args, int *i,
	t_token *cur, t_env_list *env)
{
	args[*i] = expand_token_value(cur->value, env);
	if (!args[*i])
		return (false);
	(*i)++;
	return (true);
}

char	**collect_args(t_token *start, t_token *end, int *argc, t_env_list *env)
{
	char		**args;
	t_token		*cur;
	t_token		*prev;
	int			i;

	if (!start || !end || !argc)
		return (NULL);
	args = malloc(sizeof(char *) * ((*argc = count_args(start, end)) + 1));
	if (!args)
		return (NULL);
	cur = start;
	prev = NULL;
	i = 0;
	while (cur && cur != end->next)
	{
		if (should_expand(cur, prev))
		{
			if (!assign_expanded_token(args, &i, cur, env))
				return (free_args(args, i), NULL);
		}
		prev = cur;
		cur = cur->next;
	}
	args[i] = NULL;
	return (args);
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
// 		else
// 			printf("(unknown) ");

// 		if (redir->is_heredoc && redir->delimiter)
// 		{
// 			printf("%s (delimiter)", redir->delimiter);
// 			printf(" (quoted: %s)", redir->heredoc_quoted ? "true" : "false");
// 		}
// 		else if (redir->filename)
// 			printf("%s", redir->filename);
// 		else
// 			printf("(null)");
// 		printf("\n");
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
