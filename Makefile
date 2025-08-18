NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR)

LIBFT_DIR =		srcs/utils/nolibft
LIBFT =			$(LIBFT_DIR)/libft.a
SRC = minishell.c
SRCS = srcs/env_var/env_list.c \
		srcs/env_var/env_list2.c \
		srcs/env_var/env_list3.c \
		srcs/env_var/env_util.c \
		srcs/parse/parsing.c \
		srcs/parse/help.c \
		srcs/parse/token.c \
		srcs/parse/tree.c \
		srcs/parse/tree_help.c \
		srcs/parse/redir.c \
		srcs/parse/redir2.c \
		srcs/parse/var_expansion.c \
		srcs/parse/var_expansion2.c \
		srcs/execute/builtin.c \
		srcs/execute/hdoc.c \
		srcs/execute/hdoc2.c \
		srcs/execute/hdoc_var.c \
		srcs/execute/exec_help.c \
		srcs/execute/exec.c \
		srcs/execute/exec_cmd.c \
		srcs/execute/exec_env.c \
		srcs/builtins/cd.c \
		srcs/builtins/cd2.c \
		srcs/builtins/echo.c \
		srcs/builtins/pwd.c \
		srcs/builtins/exit.c \
		srcs/builtins/env.c \
		srcs/builtins/export.c \
		srcs/builtins/export2.c \
		srcs/builtins/export3.c \
		srcs/builtins/unset.c \
		srcs/utils/utils.c \
		srcs/utils/utils2.c \
		srcs/utils/syntax.c \
		srcs/utils/syntax2.c \
		srcs/utils/syntax_help.c \
		srcs/banner.c \
		srcs/free.c \
		srcs/signals.c
		 
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o) $(SRCS:.c=.o))

$(OBJ_DIR)/%.o: %.c minishell.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@echo "✅ $(NAME) compiled successfully."

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

clean:
	@rm -f $(OBJ)
	@make --no-print-directory clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@make --no-print-directory fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re