/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:28:32 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/18 15:07:11 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*my_getenv(char *name, char **envp)
{
	int		i;
	int		len;

	i = 0;
	while (envp[i])
	{
		len = 0;
		while (envp[i][len] && envp[i][len] != '=')
			len++;
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	env_list_size(t_env_list *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->type && env->name)
			count++;
		env = env->next;
	}
	return (count);
}

char	*join_env_entry(t_env_list *node)
{
	char	*tmp;
	char	*joined;

	if (!node || !node->type || !node->name)
		return (NULL);
	tmp = ft_strjoin(node->type, "=");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, node->name);
	free(tmp);
	return (joined);
}

char	**env_list_to_envp(t_env_list *env)
{
	char		**envp;
	char		*entry;
	int			i;

	envp = malloc(sizeof(char *) * ((env_list_size(env)) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->type && env->name)
		{
			entry = join_env_entry(env);
			if (!entry)
			{
				while (i--)
					free(envp[i]);
				return (free(envp), NULL);
			}
			envp[i++] = entry;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	handle_execve_error(char *original_cmd, char *resolved_path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(original_cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free(resolved_path);
	if (errno == EACCES || errno == EPERM || errno == EISDIR
		|| errno == ENOEXEC || errno == ETXTBSY)
		exit(126);
	else
		exit(127);
}
