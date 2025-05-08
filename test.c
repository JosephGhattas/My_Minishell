#include "minishell.h"

void test_builtin(char *label, int argc, char **argv, char ***envp) {
    printf("=== [%s] ===\n", label);
    run_builtin(argc, argv, envp);
    printf("\n");
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    char **my_env = copy_env(envp);

    // ──────────────────────────────── TEST ECHO
    char *echo_args[] = {"echo", "-n", "Hello,", "world!", NULL};
    test_builtin("echo", 4, echo_args, &my_env);

    // ──────────────────────────────── TEST PWD
    char *pwd_args[] = {"pwd", NULL};
    test_builtin("pwd", 1, pwd_args, &my_env);

    // ──────────────────────────────── TEST ENV (before export)
    char *env_args[] = {"env", NULL};
    test_builtin("env (before export)", 1, env_args, &my_env);

    // ──────────────────────────────── TEST EXPORT
    char *export_args[] = {"export", "FOO=bar", "USER=testuser", NULL};
    test_builtin("export", 3, export_args, &my_env);

    // ──────────────────────────────── TEST ENV (after export)
    test_builtin("env (after export)", 1, env_args, &my_env);

    // ──────────────────────────────── TEST UNSET
    char *unset_args[] = {"unset", "FOO", NULL};
    test_builtin("unset", 2, unset_args, &my_env);

    // ──────────────────────────────── TEST ENV (after unset)
    test_builtin("env (after unset)", 1, env_args, &my_env);

    // ──────────────────────────────── TEST CD
    char *cd_args[] = {"cd", "..", NULL};
    test_builtin("cd", 2, cd_args, &my_env);

    // ──────────────────────────────── TEST PWD after CD
    test_builtin("pwd (after cd)", 1, pwd_args, &my_env);

    // ❗ Skip exit in test unless you want the program to end
    // char *exit_args[] = {"exit", "42", NULL};
    // test_builtin("exit", 2, exit_args, &my_env);

    // ──────────────────────────────── Clean up
    for (int i = 0; my_env[i]; i++)
        free(my_env[i]);
    free(my_env);

    return 0;
}
