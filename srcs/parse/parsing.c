/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:25:33 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:38:28 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_ast_node	*pipe_node(t_token *start, t_token *end, t_token *pipe_tok, t_env_list *env)
{
	t_ast_node	*node;

	if (!start || !end || !pipe_tok->prev || !pipe_tok->next)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->args = NULL;
	node->argc = 0;
	node->redirections = NULL;
	if (!pipe_tok->prev || !pipe_tok->next)
	{
		free(node);
		return (NULL);
	}
	node->left = parse_tokens(start, pipe_tok->prev, env);
	node->right = parse_tokens(pipe_tok->next, end, env);
	return (node);
}

t_ast_node	*parse_simple_command(t_token *start, t_token *end, t_env_list *env)
{
	t_ast_node	*node;

	if (!start || !end)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->args = collect_args(start, end, &node->argc, env);
	node->redirections = collect_redirections(start, end, env);
	return (node);
}

t_ast_node	*parse_tokens(t_token *start, t_token *end, t_env_list *env)
{
	t_token	*pipe_tok;

	if (!start || !end)
		return (NULL);
	if (start == end && start->type != TOKEN_PIPE)
		return (parse_simple_command(start, end, env));
	pipe_tok = find_last_token_of_type(start, end, TOKEN_PIPE);
	if (pipe_tok)
	{
		if (!pipe_tok->prev || !pipe_tok->next)
			return (NULL);
		return (pipe_node(start, end, pipe_tok, env));
	}
	return (parse_simple_command(start, end, env));
}

t_ast_node	*parse_input(char *input, t_env_list *my_env)
{
	t_token		*token;
	t_ast_node	*tree;

	(void)my_env;
	token = tokenize_input(input);
	tree = parse_tokens(find_first_token(token), find_last_token(token), my_env);
	// print_tokens(token);
	print_ast(tree, 0);
	return (tree);
}
