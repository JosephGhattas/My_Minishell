NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR)

LIBFT_DIR =		srcs/utils/nolibft
LIBFT =			$(LIBFT_DIR)/libft.a
SRC = minishell.c
SRCS = srcs/execute/builtin.c \
		srcs/builtins/cd.c \
		srcs/builtins/env.c \
		srcs/builtins/exit.c \
		srcs/builtins/export.c \
		srcs/builtins/pwd_echo.c \
		srcs/builtins/unset.c \
		srcs/execute.c \
		srcs/banner.c \
		srcs/free.c \
		srcs/execute/heredoc.c \
		srcs/execute/pipes.c \
		srcs/execute/signals.c \
		srcs/parse/parsing.c 

OBJ = $(SRCS:.c=.o) $(SRC:.c=.o)
%.o: %.c minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: srcs/%.c minishell.h
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