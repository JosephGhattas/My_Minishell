/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:29:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 14:33:20 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*append_char(char *result, char c)
{
	char	*tmp;
	char	*new;

	tmp = ft_char_to_str(c);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	new = ft_strjoin(result, tmp);
	free(result);
	free(tmp);
	return (new);
}

char	*generate_heredoc_filename(void)
{
	static int	counter = 0;
	char		*number;
	char		*name;
	char		*prefix;

	number = ft_itoa(counter++);
	if (!number)
		return (NULL);
	prefix = ft_strdup("/tmp/.heredoc_tmp_");
	if (!prefix)
		return (free(number), NULL);
	name = ft_strjoin(prefix, number);
	free(prefix);
	free(number);
	return (name);
}

int	fd_failed(t_redir	*redir, bool open)
{
	free(redir->filename);
	redir->filename = NULL;
	if (open)
		perror("open");
	return (-1);
}

int	setup_heredocs(t_redir *list, t_env_list *env)
{
	int	ret;

	while (list)
	{
		if (list->type == TOKEN_HEREDOC && list->delimiter)
		{
			ret = create_heredoc_file(list, env);
			if (ret != 0)
			{
				if (ret == 130)
					return (130);
				else
				{
					if (g_sig != SIGINT)
						ft_putstr_fd("minishell: heredoc failed\n", 2);
					return (1);
				}
			}
		}
		list = list->next;
	}
	return (0);
}

int	setup_all_heredocs(t_ast_node *node, t_env_list *env)
{
	int	ret;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		ret = setup_heredocs(node->redirections, env);
		if (ret != 0)
			return (ret);
	}
	if (node->left)
	{
		ret = setup_all_heredocs(node->left, env);
		if (ret != 0)
			return (ret);
	}
	if (node->right)
	{
		ret = setup_all_heredocs(node->right, env);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
