#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

	my_env = copy_env(envp);
    (void)argc;
	(void)argv;
	(void)my_env;
    printbanner();
    //temporary parsing
	t_command	cmd1 =
    {
        .argv = (char *[]){"env", NULL, NULL},
        .infile = NULL,
        .outfile = "out.txt",
        .append = 0,
        .next = NULL
    };

    /*// Simulate: cat < input.txt | grep hello | wc -l > out.txt
    t_command c1 = { .argv = (char *[]){"cat", NULL}, .infile = "input.txt", .next = &c2 };
    t_command c2 = { .argv = (char *[]){"grep", "hello", NULL}, .next = &c3 };
    t_command c3 = { .argv = (char *[]){"wc", "-l", NULL}, .outfile = "out.txt", .append = 0 };
    */

    execute_command(&cmd1);
    return (0);
}
