NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.


SRC = minishell.c
SRCS = srcs/builtin.c srcs/builtins/cd.c srcs/builtins/env.c srcs/execute.c srcs/builtins/exit.c srcs/builtins/export.c srcs/builtins/pwd_echo.c srcs/builtins/unset.c

OBJ = $(SRC:.c=.o) $(SRCS:.c=.o)
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: srcs/%.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re