/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:56:36 by jgh               #+#    #+#             */
/*   Updated: 2025/07/25 13:01:54 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	memory_error(void)
{
	perror("error in memory allocation");
	exit(EXIT_FAILURE);
}

void	check_shell_number(int *shell_number)
{
	if ((*shell_number) <= 0)
		(*shell_number) = 1;
	else
		(*shell_number) += 1;
	if ((*shell_number) > INT_MAX)
		(*shell_number) = INT_MAX;
	else
		(*shell_number) = (*shell_number);
}

void	new_node_filling(t_env_list **new_node, t_env_list **head)
{
	(*new_node)->type = ft_strdup("SHLVL");
	(*new_node)->name = ft_strdup("1");
	(*new_node)->equal = true;
	(*new_node)->heredoc_filename = NULL;
	(*new_node)->exit_status = 0;
	(*new_node)->shell_pwd = NULL;
	(*new_node)->shell_oldpwd = NULL;
	(*new_node)->next = (*head);
	(*new_node)->prev = NULL;
}

t_env_list	*add_shell_level(t_env_list *env_list)
{
	t_env_list	*head;
	t_env_list	*new_node;
	int			shell_level;

	head = env_list;
	while (env_list)
	{
		if (ft_strcmp(env_list->type, "SHLVL") == 0)
		{
			shell_level = ft_atoi(env_list->name);
			check_shell_number(&shell_level);
			free(env_list->name);
			env_list->name = ft_itoa(shell_level);
			if (!env_list->name)
				memory_error();
			return (head);
		}
		env_list = env_list->next;
	}
	new_node = malloc(sizeof(t_env_list));
	if (!new_node)
		memory_error();
	new_node_filling(&new_node, &head);
	if (head)
		head->prev = new_node;
	return (head);
}
