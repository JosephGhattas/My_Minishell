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

t_token	*parse_input(char *input, t_env_list *my_env)
{
	t_token	*tokens;

	(void)my_env;
	tokens = tokenize_input(input);
	print_tokens(tokens);
	return (tokens);
}
