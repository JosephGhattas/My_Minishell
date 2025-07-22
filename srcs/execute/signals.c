/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:23:27 by jghattas          #+#    #+#             */
/*   Updated: 2025/07/21 11:23:31 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <signal.h>
#include <readline/readline.h>

volatile sig_atomic_t g_sig = 0;

void	sig_handler_prompt(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		write(1, "\b\b  \b\b", 6);
	}
}

void	sig_handler_exec(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		g_sig = 128 + sig;
		write(1, "\n", 1);
	}
}

void	setup_signals_prompt(void)
{
	signal(SIGINT, sig_handler_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_exec(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
