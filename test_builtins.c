#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h" // Make sure this includes your function prototypes

int main(int argc, char **argv, char **envp) {
    char **my_env = copy_env(envp);

    // Test `export`
    char *export_args[] = {"export", "FOO=bar", "HELLO=world", NULL};
    run_builtin(3, export_args, &my_env);

    // Test `env`
    char *env_args[] = {"env", NULL};
    run_builtin(1, env_args, &my_env);

    // Test `unset`
    char *unset_args[] = {"unset", "FOO", NULL};
    run_builtin(2, unset_args, &my_env);

    // Test `env` again to see changes
    run_builtin(1, env_args, &my_env);

    // Test `cd`
    char *cd_args[] = {"cd", "..", NULL};
    run_builtin(2, cd_args, &my_env);

    // Test `pwd`
    char *pwd_args[] = {"pwd", NULL};
    run_builtin(1, pwd_args, &my_env);

    // Free env manually (if not using a cleanup func)
    for (int i = 0; my_env[i]; i++)
        free(my_env[i]);
    free(my_env);

    return 0;
}
