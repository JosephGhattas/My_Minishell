/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:27:21 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/12 11:49:25 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_home_path(t_env_list *env)
{
	char	*home;

	home = get_env_value(env, "HOME");
	if (!home || !*home)
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	if (!home)
		return (NULL);
	return (ft_strdup(home));
}

static char	*get_oldpwd_path(t_env_list *env)
{
	char	*oldpwd;

	oldpwd = get_env_value(env, "OLDPWD");
	if (!oldpwd || !*oldpwd)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
	if (!oldpwd)
		return (NULL);
	return (ft_strdup(oldpwd));
}

static char	*get_target_path(int argc, char **argv, t_env_list *env)
{
	if (argc == 1)
		return (get_home_path(env));
	if (ft_strcmp(argv[1], "-") == 0)
		return (get_oldpwd_path(env));
	return (ft_strdup(argv[1]));
}

static void	update_pwd_env(t_env_list **env, char *oldpwd, char *target)
{
	char	*newpwd;

	if (oldpwd)
		update_env_var(env, "OLDPWD", oldpwd);
	else
		update_env_var(env, "OLDPWD", "");
	newpwd = safe_getcwd();
	if (!newpwd)
	{
		if (target[0] == '/')
			newpwd = ft_strdup(target);
		else if (oldpwd)
			newpwd = ft_strjoin3(oldpwd, "/", target);
		else
			newpwd = ft_strdup(target);
	}
	if (newpwd)
	{
		update_env_var(env, "PWD", newpwd);
		free(newpwd);
	}
	else
		update_env_var(env, "PWD", "");
}

int	my_cd(int argc, char **argv, t_env_list **env)
{
	char	*oldpwd;
	char	*target;
	int		ret;

	if (argc > 2)
		return (handle_too_many_args(argc));
	oldpwd = get_env_value(*env, "PWD");
	if (oldpwd)
		oldpwd = ft_strdup(oldpwd);
	else
		oldpwd = safe_getcwd();
	target = get_target_path(argc, argv, *env);
	if (!target)
		return (free(oldpwd), 1);
	ret = chdir(target);
	if (ret == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(target);
		return (free(oldpwd), free(target), 1);
	}
	update_pwd_env(env, oldpwd, target);
	if (argc == 2 && !ft_strcmp(argv[1], "-"))
		ft_putendl_fd(get_env_value(*env, "PWD"), STDOUT_FILENO);
	return (free(oldpwd), free(target), 0);
}
