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
# include <signal.h>
# include <limits.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

// # define PATH_MAX 2000

extern volatile sig_atomic_t g_sig;

typedef struct s_env_list
{
    char              *name;              // value
    char              *type;              // key
    bool              equal;              // true if '=' exists in original string
    char              *heredoc_filename;  // for heredoc temp file
	int               exit_status;        // exit status of the last command
    char              *shell_pwd;      	// shell's PWD
    char              *shell_oldpwd;   // shell's OLDPWD
    struct s_env_list *next;
    struct s_env_list *prev;
}                 t_env_list;

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


//signals
void		sig_handler_prompt(int sig);
void		sig_handler_exec(int sig);
void		setup_signals_prompt(void);
void		setup_signals_exec(void);

//env_list
void		init(int argc, char **argv, t_env_list **env_list, char **env);
void		cheaking_env(t_env_list **env_list, char **env);
void		memory_error(void);
void		check_shell_number(int *shell_number);
void		new_node_filling(t_env_list **new_node, t_env_list **head);
t_env_list	*add_shell_level(t_env_list *env_list);
char		*extract_value_part(char *env);
char		*extract_key_part(char *env_var);
bool		equal(char *env);
t_env_list	*create_env_node(char *env);
void		append_env_node(t_env_list **current, t_env_list **new_node);
t_env_list	*add_shell_level(t_env_list *env_list);
t_env_list	*generate_env_list(char **env);
t_env_list	*create_default_env(void);

//parse
t_token		*parse_input(char *input, t_env_list *my_env);
t_token		*tokenize_input(const char *line);
t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_tok);
void		print_tokens(t_token *token);

//help parse
int			ft_isspace(char c);
int			is_metachar(char c);
char 		*ft_char_to_str(char c);
char 		*ft_strjoin_free(char *s1, char *s2);
void		read_operator(const char *line, size_t *i, t_token **tokens);
void		read_word(const char *line, size_t *i, t_token **tokens);

//execute
int			is_builtin(char *cmd);
int			run_builtin(int argc, char **argv, char ***envp);
int			execute_command(t_command *cmd);
char		*find_path(char *cmd, char **envp);
int			execute_pipeline(t_command *cmd);
int			setup_heredoc(t_command *cmd);
int			handle_all_heredocs(t_command *cmd);

//builtin
int			my_cd(int argc, char **argv);
int			my_pwd(void);
int			my_echo(int argc, char**argv);
char		**copy_env(char **envp);
int			my_env(int argc, char **argv, char ***my_env);
int			my_export(int argc, char **argv, char ***envp);
int			my_unset(int argc, char **argv, char ***envp);
int			my_exit(int argc, char **argv);

//cleanup
void		free_array(char **arr);
void		free_command(t_command *cmd);
void 		free_env_list_full(t_env_list *env);
//rnd
void		printbanner(void);

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