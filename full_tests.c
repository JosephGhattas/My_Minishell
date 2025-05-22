#include "minishell.h"

static void simulate_echo(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"echo", "-n", "Hello,", "world!", NULL},
		.argc = 4, .infile = NULL, .outfile = NULL,
		.append = 0, .heredoc_delim = NULL, .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

static void simulate_redirection_out(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"echo", "Redirected", NULL},
		.argc = 2, .infile = NULL, .outfile = "test_out.txt",
		.append = 0, .heredoc_delim = NULL, .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

static void simulate_redirection_append(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"echo", "Appended", NULL},
		.argc = 2, .infile = NULL, .outfile = "test_out.txt",
		.append = 1, .heredoc_delim = NULL, .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

static void simulate_redirection_in(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"cat", NULL},
		.argc = 1, .infile = "test_out.txt", .outfile = NULL,
		.append = 0, .heredoc_delim = NULL, .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

static void simulate_pipeline(char **env)
{
    t_command c3 = {
        .argv = (char *[]){"wc", "-l", NULL}, .argc = 2,
        .infile = NULL, .outfile = NULL, .append = 0,
        .heredoc_delim = NULL, .envp = env, .next = NULL
    };
    t_command c2 = {
        .argv = (char *[]){"grep", "main", NULL}, .argc = 2,
        .infile = NULL, .outfile = NULL, .append = 0,
        .heredoc_delim = NULL, .envp = env, .next = &c3
    };
    t_command c1 = {
        .argv = (char *[]){"cat", "minishell.c", NULL}, .argc = 3,
        .infile = NULL, .outfile = NULL, .append = 0,
        .heredoc_delim = NULL, .envp = env, .next = &c2
    };
    execute_command(&c1);
}


static void simulate_heredoc(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"cat", NULL},
		.argc = 1, .infile = NULL, .outfile = NULL,
		.append = 0, .heredoc_delim = "END", .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

static void simulate_exit(char **env)
{
	t_command cmd = {
		.argv = (char *[]){"exit", "0", NULL},
		.argc = 2, .infile = NULL, .outfile = NULL,
		.append = 0, .heredoc_delim = NULL, .envp = env, .next = NULL
	};
	execute_command(&cmd);
}

int	main(int argc, char **argv, char **envp)
{
    (void)argv;
    (void)argc;
	char *input;
	char **my_env = copy_env(envp);

	while (1)
	{
		setup_signals_prompt();
		input = readline("test$ ");
		if (!input)
			break;
		if (!strcmp(input, "echo")) simulate_echo(my_env);
		else if (!strcmp(input, "redir_out")) simulate_redirection_out(my_env);
		else if (!strcmp(input, "redir_app")) simulate_redirection_append(my_env);
		else if (!strcmp(input, "redir_in")) simulate_redirection_in(my_env);
		else if (!strcmp(input, "pipe")) simulate_pipeline(my_env);
		else if (!strcmp(input, "heredoc")) simulate_heredoc(my_env);
		else if (!strcmp(input, "exit")) simulate_exit(my_env);
		else printf("Unknown test: %s\n", input);
		free(input);
	}

	for (int i = 0; my_env[i]; i++) free(my_env[i]);
	free(my_env);
	printf("Exiting...\n");
	return 0;
}
