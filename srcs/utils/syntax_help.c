/* helpers.c */

#include "minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <stdbool.h>

/*
** Return true if there is an unclosed ' or " in s (silent version).
*/
bool	detect_unclosed_quotes_silent(const char *s)
{
	char	q;
	int		i;

	q = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (q == 0)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		i++;
	}
	return (q != 0);
}

/*
** Return true if the last non-space, unquoted char is '|' (needs continuation).
*/
bool	detect_trailing_pipe(const char *s)
{
	bool	last_pipe;
	char	quote;
	int		i;

	last_pipe = false;
	quote      = 0;
	i          = 0;
	while (s[i] && ft_isspace((unsigned char)s[i]))
		i++;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (quote == 0)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		else if (s[i] == '|' && quote == 0)
			last_pipe = true;
		else if (!ft_isspace((unsigned char)s[i]) && quote == 0)
			last_pipe = false;
		i++;
	}
	return (last_pipe);
}

/*
** Read lines until quotes and trailing-pipe are closed, then return full buf.
*/
char	*read_complete_input(void)
{
	char	*buf;
	char	*line;

	buf = readline("CJminishell$ ");
	if (!buf)
		return (NULL);
	while (detect_unclosed_quotes_silent(buf)
		|| detect_trailing_pipe(buf))
	{
		line = readline("> ");
		if (!line)
		{
			free(buf);
			return (NULL);
		}
		buf = ft_strjoin_free(buf, "\n");
		buf = ft_strjoin_free(buf, line);
		free(line);
	}
	return (buf);
}

/*
** Only error on consecutive unquoted pipes: "||" or "| |".
*/
bool	detect_consecutive_pipes(const char *s)
{
	bool	last_pipe;
	char	quote;
	int		i;

	last_pipe = false;
	quote      = 0;
	i          = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (quote == 0)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		else if (s[i] == '|' && quote == 0)
		{
			if (last_pipe)
			{
				ft_putstr_fd(
			"CJminishell: syntax error near unexpected token `|'\n", 2);
				return (true);
			}
			last_pipe = true;
		}
		else if (!ft_isspace((unsigned char)s[i]) && quote == 0)
			last_pipe = false;
		i++;
	}
	return (false);
}
