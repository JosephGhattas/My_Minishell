#include <string.h> //replace strcmp with my own
#include "minishell.h"

int	is_builtin(const char *cmd)
{
    if (!cmd)
		return (0);
    return (!strcmp(cmd, "cd") ||
            !strcmp(cmd, "echo") ||
            !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "export") ||
            !strcmp(cmd, "unset") ||
            !strcmp(cmd, "env") ||
            !strcmp(cmd, "exit"));
}

int run_builtin(int argc, char **argv, char **envp)
{
    if (!argv || !argv[0])
        return (1);
    if (!strcmp(argv[0], "cd"))
        return (my_cd(argc, argv));
    else if (!strcmp(argv[0], "echo"))
        return (my_echo(argc, argv));
    else if (!strcmp(argv[0], "pwd"))
        return (my_pwd());
    else if (!strcmp(argv[0], "export"))
        return (my_export(argc, argv, envp));
    else if (!strcmp(argv[0], "unset"))
        return (my_unset(argc, argv, envp));
    else if (!strcmp(argv[0], "env"))
        return (my_env(argc, argv, envp));
    else if (!strcmp(argv[0], "exit"))
    {
        my_exit(argc, argv);
        return (0);
    }
    return (1);
}
