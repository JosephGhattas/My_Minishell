/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd&echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:41:25 by jghattas          #+#    #+#             */
/*   Updated: 2025/03/28 11:56:08 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	my_echo(int argc, char**argv)
{
	int	no_line;
	int	i;

	no_line = 0;
	i = 1;
	if (argc > 1 && strcmp(argv[1], "-n") == 0)
	{
		no_line = 1;
		argc--;
		argv++;
	}
	while (i < argc)
	{
		printf("%s", argv[i]);
		i++;
		if (i < argc - 1)
			printf(" ");
	}
	if (no_line == 0)
		printf("\n");
	return (0);
}

int	my_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
