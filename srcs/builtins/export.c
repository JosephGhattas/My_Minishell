/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:31:14 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/08 14:47:45 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:31:14 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/01 15:11:00 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env_list	*find_env_var_export(t_env_list *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->type, (char *)key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	append_env_node_export(t_env_list **env, t_env_list *new)
{
	t_env_list	*last;

	if (!*env)
	{
		*env = new;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

t_env_list	*create_env_node_export(char *key, char *val, bool equal)
{
	t_env_list	*new;

	new = malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->type = key;
	new->name = val;
	new->equal = equal;
	new->heredoc_filename = NULL;
	new->exit_status = 0;
	new->shell_pwd = NULL;
	new->shell_oldpwd = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	update_or_add_env_export(t_env_list **env, const char *arg)
{
	char		*eq;
	char		*key;
	char		*val;
	t_env_list	*existing;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		existing = find_env_var_export(*env, arg);
		if (existing)
			existing->equal = true;
		else
			append_env_node_export(env, create_env_node_export(ft_strdup((char*)arg), ft_strdup(""), true));
		return (0);
	}
	key = ft_substr(arg, 0, eq - arg);
	val = ft_strdup(eq + 1);
	if (!key || !val)
		return (free(key), free(val), 1);
	existing = find_env_var_export(*env, key);
	if (existing)
	{
		free(existing->name);
		existing->name = val;
		existing->equal = true;
		free(key);
	}
	else
		append_env_node_export(env, create_env_node_export(key, val, true));
	return (0);
}

static void	print_one_export(t_env_list *node)
{
	char	*line;
	char	*tmp;

	if (node->equal)
	{
		/* build: declare -x KEY="VALUE" */
		line = ft_strjoin("declare -x ", node->type);
		tmp  = ft_strjoin(line, "=\"");
		free(line);
		line = ft_strjoin(tmp, node->name);
		free(tmp);
		tmp  = ft_strjoin(line, "\"\n");
		free(line);
		ft_putstr_fd(tmp, 1);
		free(tmp);
	}
	else
	{
		line = ft_strjoin("declare -x ", node->type);
		ft_putendl_fd(line, 1);
		free(line);
	}
}

static void	print_sorted_env(t_env_list *env)
{
	t_env_list	**arr;
	t_env_list	*tmp;
	int			count;
	int			i;
	int			j;
	t_env_list	*swap;

	count = 0;
	tmp   = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(count * sizeof(t_env_list *));
	if (!arr)
		return ;
	tmp = env;
	i   = 0;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp      = tmp->next;
	}
	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->type, arr[j]->type) > 0)
			{
				swap   = arr[i];
				arr[i] = arr[j];
				arr[j] = swap;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		print_one_export(arr[i]);
		i++;
	}
	free(arr);
}

int my_export(int argc, char **argv, t_env_list **env)
{
    int     i;
    char    *key;
    char    *eq_pos;

	if (argc == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
    i = 1;
    while (i < argc)
    {
        eq_pos = ft_strchr(argv[i], '=');
        if (eq_pos)
            key = ft_strndup(argv[i], eq_pos - argv[i]);
        else
            key = ft_strdup(argv[i]);
		if (!key)
			return (1);
        if (!is_valid_identifier(key))
        {
            printf("export: `%s': not a valid identifier\n", argv[i]);
            free(key);
            i++;
            continue ;
        }
        free(key);
        if (update_or_add_env_export(env, argv[i]) != 0)
            return (1);
        i++;
    }
    return (0);
}
