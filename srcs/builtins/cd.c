/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:27:21 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/05 15:27:23 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	cd_to_target(char *target)
{
	if (!target)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	cd_to_oldpwd(t_env_list *env)
{
	char	*oldpwd;

	oldpwd = get_env_value(env, "OLDPWD");
	if (!oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(oldpwd) == -1)
	{
		perror("cd");
		return (1);
	}
	printf("%s\n", oldpwd);
	return (0);
}

char	*cd_handle_path(int argc, char **argv, t_env_list *env)
{
	if (argc == 1)
		return (get_env_value(env, "HOME"));
	if (argc == 2 && ft_strcmp(argv[1], "-") == 0)
		return (NULL);
	return (argv[1]);
}

int	my_cd_change_dir(int argc, char **argv,
		t_env_list **env, char **oldpwd)
{
	int		ret;
	char	*target;

	*oldpwd = safe_getcwd();
	if (!*oldpwd)
	{
		perror("cd");
		return (-1);
	}
	if (argc == 2 && ft_strcmp(argv[1], "-") == 0)
		ret = cd_to_oldpwd(*env);
	else
	{
		target = cd_handle_path(argc, argv, *env);
		ret = cd_to_target(target);
	}
	return (ret);
}

int	my_cd(int argc, char **argv, t_env_list **env)
{
	char	*oldpwd;
	char	*cwd;
	int		ret;

	if (!env)
		return (1);
	ret = my_cd_change_dir(argc, argv, env, &oldpwd);
	if (ret == -1)
		return (1);
	if (ret != 0)
		return (free(oldpwd), ret);
	cwd = safe_getcwd();
	if (!cwd)
	{
		free(oldpwd);
		perror("cd");
		return (1);
	}
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", cwd);
	free(oldpwd);
	free(cwd);
	return (0);
}
