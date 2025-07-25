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

void	free_command(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = tmp;
	}
}

void	free_env_list_full(t_env_list *env)
{
	t_env_list	*current;
	t_env_list	*next;

	if (env == NULL)
		return ;
	current = env;
	while (current != NULL)
	{
		next = current->next;
		if (current->name != NULL)
			free(current->name);
		if (current->type != NULL)
			free(current->type);
		if (current->heredoc_filename != NULL)
			free(current->heredoc_filename);
		if (current->shell_pwd != NULL)
			free(current->shell_pwd);
		if (current->shell_oldpwd != NULL)
			free(current->shell_oldpwd);
		free(current);
		current = next;
	}
}
