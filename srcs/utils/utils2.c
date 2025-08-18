/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:16:44 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 17:16:45 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	frees(t_ast_node *tree, t_env_list *env)
{
	free_ast(tree);
	free_env_list_full(env);
	rl_clear_history();
}

void	main_free(t_env_list *env)
{
	free_env_list_full(env);
	rl_clear_history();
}
