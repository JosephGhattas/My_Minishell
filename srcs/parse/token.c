/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:07:49 by marvin            #+#    #+#             */
/*   Updated: 2025/07/22 10:07:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}

void	add_token(t_token **head, t_token *new_tok)
{
	t_token	*cur;

	cur = *head;
	if (!*head)
	{
		*head = new_tok;
		return ;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = new_tok;
	new_tok->prev = cur;
}

t_token	*tokenize_input(const char *line)
{
	t_token	*tokens;
	size_t	i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			read_operator(line, &i, &tokens);
		else
			read_word(line, &i, &tokens);
	}
	return (tokens);
}

// void	print_tokens(t_token *token)
// {
// 	while (token)
// 	{
// 		printf("TOKEN(%d): [%s]\n", token->type, token->value);
// 		token = token->next;
// 	}
// 	printf("\n\n\n");
// }
