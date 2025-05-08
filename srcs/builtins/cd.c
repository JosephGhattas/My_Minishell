/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:39:46 by jghattas          #+#    #+#             */
/*   Updated: 2025/03/27 13:40:16 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	get_current_dir(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	cd2(char **argv, char *oldpwd)
{
	if (strcmp(argv[1], "-") == 0)
	{
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
	else
	{
		if (chdir(argv[1]) == -1)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
}

static int	cd_home(char *home)
{
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	my_cd(int argc, char **argv)
{
	char	oldpwd[PATH_MAX];
	char	cwd[PATH_MAX];
	int		ret;

	ret = 0;
	if (get_current_dir(oldpwd, sizeof(oldpwd)))
		return (1);
	if (argc == 2)
		ret = cd2(argv, oldpwd);
	else if (argc == 1)
		ret = cd_home(getenv("HOME"));
	else
	{
		printf("cd: Usage: cd <directory>\n");
		return (1);
	}
	if (ret == 0)
	{
		setenv("OLDPWD", oldpwd, 1);
		if (get_current_dir(cwd, sizeof(cwd)))
			return (1);
		setenv("PWD", cwd, 1);
	}
	return (ret);
}
