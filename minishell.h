#ifndef MINISHELL_H
# define MINISHELL_H

# include "srcs/utils/nolibft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>



# define PATH_MAX 2000
typedef struct	s_command
{
    int		argc;
    char	**argv;
    char	*infile;
    char	*outfile;
    int		append;
	char	**envp;
    struct s_command	*next;
}	t_command;

//execute
int		is_builtin(const char *cmd);
int		run_builtin(int argc, char **argv, char ***envp);
int		execute_command(t_command *cmd);
char	*find_path(char *cmd, char **envp);

//builtin
int		my_cd(int argc, char **argv);
int		my_pwd(void);
int		my_echo(int argc, char**argv);
char	**copy_env(char **envp);
int		my_env(int argc, char **argv, char ***my_env);
int		my_export(int argc, char **argv, char ***envp);
int		my_unset(int argc, char **argv, char ***envp);
int		my_exit(int argc, char **argv);

//cleanup
void	free_array(char **arr);
//rnd
void	printbanner(void);

//ansi colors
//============================================
# define RST     "\x1b[0m"
# define R       "\x1b[31m"
# define G       "\x1b[32m"
# define Y       "\x1b[33m"
# define B       "\x1b[34m"
# define C       "\x1b[36m" 
# define M		 "\x1b[35m"
//============================================
#endif