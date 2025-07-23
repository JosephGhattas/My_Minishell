#ifndef MINISHELL_H
# define MINISHELL_H

# include "srcs/utils/nolibft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>


# define PATH_MAX 2000
typedef struct	s_command
{
    int		argc;
    char	**argv;
    char	*infile;
    char	*outfile;
    int		append;
	char	**envp;
	char	*heredoc_delim;
	int		heredoc_fd;
	int		heredoc_expand;
    struct s_command	*next;
}	t_command;

typedef enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,     // <
	TOKEN_REDIR_OUT,    // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_EOF
}	t_token_type;

typedef struct	s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;


//parse
t_command	*parse_input(char *input, char **my_env);
t_token		*tokenize_input(const char *line);
t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_tok);
// void	print_tokens(t_token *token);

//help parse
int	ft_isspace(char c);
int	is_metachar(char c);
char *ft_char_to_str(char c);
char *ft_strjoin_free(char *s1, char *s2);
void	read_operator(const char *line, size_t *i, t_token **tokens);
void	read_word(const char *line, size_t *i, t_token **tokens);

//execute
int		is_builtin(char *cmd);
int		run_builtin(int argc, char **argv, char ***envp);
int		execute_command(t_command *cmd);
char	*find_path(char *cmd, char **envp);
int		execute_pipeline(t_command *cmd);
int		setup_heredoc(t_command *cmd);
int		handle_all_heredocs(t_command *cmd);

//signals
void	sig_handler_prompt(int sig);
void	sig_handler_exec(int sig);
void	setup_signals_prompt(void);
void	setup_signals_exec(void);

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
void	free_command(t_command *cmd);
void	free_env(char **env);
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