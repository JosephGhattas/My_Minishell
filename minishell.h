/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:30:18 by jghattas          #+#    #+#             */
/*   Updated: 2025/08/05 15:30:18 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

//global signals variable//
extern volatile sig_atomic_t	g_sig;

//envp variable list//
typedef struct s_env_list
{
	char				*name;// value
	char				*type;// key
	bool				equal;// true if '=' exists in original string
	char				*heredoc_filename;// for heredoc temp file
	int					exit_status;// exit status of the last command
	char				*shell_pwd;// shell's PWD
	char				*shell_oldpwd;// shell's OLDPWD
	struct s_env_list	*next;
	struct s_env_list	*prev;
}	t_env_list;

//tokens list//
typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

//AST tree//
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC
}	t_node_type;

typedef struct s_redir
{
	t_token_type	type;
	bool			is_heredoc;
	char			*filename;
	char			*delimiter;
	bool			heredoc_quoted;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	int					argc;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_redir				*redirections;
}	t_ast_node;

//debug
void		print_tokens(t_token *token);
void		print_ast(t_ast_node *node, int depth);

//syntax
bool		detect_syntax_errors(const char *input);
bool		trailing_pipe(const char *s);
bool		leading_pipe(const char *s);
bool		detect_consecutive_pipes(const char *s);
bool		detect_invalid_metachar(const char *s);
bool		is_only_whitespace(const char *s);
bool		detect_redir_errors(const char *s);
char		update_quote(char current_quote, char c);

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
void		new_node_filling(t_env_list *new_node, t_env_list *head);
t_env_list	*add_shell_level(t_env_list *env_list);
char		*extract_value_part(char *env);
char		*extract_key_part(char *env_var);
bool		equal(char *env);
t_env_list	*create_env_node(char *env);
void		append_env_node(t_env_list **current, t_env_list **new_node);
t_env_list	*add_shell_level(t_env_list *env_list);
t_env_list	*generate_env_list(char **env);
t_env_list	*create_default_env(void);

//var expansion
char		*get_special_var(char c, t_env_list *env);
char		*get_var_name(const char *s, int *len);
char		*get_env_value_exp(const char *key, t_env_list *env);
char		*expand_token_value(const char *input, t_env_list *env);
void		update_exit_status(t_env_list **env, int status);
int			get_exit_status(t_env_list *env);

//tokenization
t_token		*tokenize_input(const char *line);
t_token		*new_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_tok);

//help tokenization
int			ft_isspace(char c);
int			is_metachar(char c);
char		*ft_char_to_str(char c);
char		*ft_strjoin_free(char *s1, char *s2);
void		read_operator(const char *line, size_t *i, t_token **tokens);
void		read_word(const char *line, size_t *i, t_token **tokens);

//AST Tree
t_token		*find_last_token(t_token *token);
t_token		*find_first_token(t_token *token);
t_token		*find_last_token_of_type(t_token *start,
				t_token *end, t_token_type type);
t_redir		*collect_redirections(t_token *start,
				t_token *end, t_env_list *env);
t_redir		*new_redir(t_token *token, t_token *next);
void		append_redir(t_redir **head, t_redir **tail, t_redir *new_redir);
char		**collect_args(t_token *start, t_token *end,
				int *argc, t_env_list *env);
int			count_args(t_token *start, t_token *end);

//parse
t_ast_node	*parse_input(char *input, t_env_list *my_env);
t_ast_node	*parse_simple_command(t_token *start,
				t_token *end, t_env_list *env);
t_ast_node	*parse_tokens(t_token *start, t_token *end, t_env_list *env);
t_ast_node	*pipe_node(t_token *start, t_token *end,
				t_token *pipe_tok, t_env_list *env);

//execute

int			execute_ast(t_ast_node *node, t_env_list **env);
int			execute_pipe(t_ast_node *node, t_env_list **env);
int			execute_command_node(t_ast_node *cmd, t_env_list **env);
int			is_builtin(char *cmd);
int			run_builtin(int argc, char **argv, t_env_list **envp);

//herdocs handling
char		*generate_heredoc_filename(void);
int			create_heredoc_file(t_redir *redir);
int			setup_heredocs(t_redir *list);
int			setup_all_heredocs(t_ast_node *node);

//redirections
int			setup_redirections(t_redir *redir);
int			open_redirection_file(t_redir *redir);

//exec_env
int			env_list_size(t_env_list *env);
char		*join_env_entry(t_env_list *node);
char		**env_list_to_envp(t_env_list *env);
char		*find_path(char *cmd, char **envp);
char		*my_getenv(char *name, char **envp);

//builtins//
//=========//

//pwd
char		*safe_getcwd(void);
t_env_list	*find_env_node(t_env_list *env, char *key);
int			my_pwd(t_env_list **env);

//cd
char		*ft_strjoin3(char *s1, char *s2, char *s3);
void		append_env_node_cd(t_env_list **head, t_env_list **new_node);
void		update_env_var(t_env_list **env, char *key, char *value);
char		*get_env_value(t_env_list *env, char *key);
int			cd_to_target(char *target);
int			cd_to_oldpwd(t_env_list *env);
char		*cd_handle_path(int argc, char **argv, t_env_list *env);
int			my_cd_change_dir(int argc, char **argv,
				t_env_list **env, char **oldpwd);
int			my_cd(int argc, char **argv, t_env_list **env);

//export
int			is_valid_identifier(const char *str);
t_env_list	*find_env_var_export(t_env_list *env, const char *key);
void		append_env_node_export(t_env_list **env, t_env_list *new);
t_env_list	*create_env_node_export(char *key, char *val, bool equal);
int			update_or_add_env_export(t_env_list **env, const char *arg);
void		print_one_export(t_env_list *node);
int			my_export(int argc, char **argv, t_env_list **env);
int			handle_no_equal(t_env_list **env, const char *arg);
int			handle_with_equal(t_env_list **env, const char *arg, char *eq);
void		sort_env_array(t_env_list **arr, int count);
void		print_sorted_env(t_env_list *env);
int			process_arg(t_env_list **env, char *arg);

//unset
void		remove_env_var(t_env_list **env, const char *key);
int			my_unset(int argc, char **argv, t_env_list **env);
//
int			my_env(int argc, char **argv, t_env_list **env);
int			my_echo(int argc, char**argv);

int			my_exit(int argc, char **argv, t_env_list **env);

//cleanup
void		free_array(char **arr);
void		free_env_list_full(t_env_list *env);
void		free_tokens(t_token *tok);
void		free_single_redir(t_redir *redir);
void		free_redir_list(t_redir *head);

void		free_ast(t_ast_node *node);

//rnd
void		printbanner(void);
char		*ft_strndup(char *src, unsigned int n);

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