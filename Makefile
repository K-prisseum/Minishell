
NAME        := minishell
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -Iinclude -Ilibft
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a
SRC_DIR     := src
OBJ_DIR     := obj


SRC_FILES   := main.c \
               signals.c \
               prompt.c\
               init.c\
               env.c\
               env_utils.c\
               utils.c\
               input.c\
               input_reader.c\
               token_utils.c\
               tokenize.c\
               tokenizer_word.c\
               tokenizer_heredoc.c\
               utils_parser.c\
               quotes.c\
               validate_token.c\
               init_commands.c\
               organize_com_utils.c\
               organize_commands.c\
               expand_variables.c\
               handles_quotes_exp.c\
               cleanup.c\
               print_utils.c\
               debug.c\
               builtin_exit.c\
               builtin_export.c\
               builtin_unset.c\
               builtin_env.c\
               builtin_pwd.c\
               builtin_utils.c\
               safe_utils.c\
               heredoc.c\
               heredoc_setup.c\
               execute_commands.c\
               execute_status.c\
               execute_child.c\
               execute_redirections.c\
               resolve_command.c\
               handle_redirections.c\
               execute_builtin.c\
               builtin_cd.c\
               cd_utils_a.c\
               cd_utils_b.c\
               builtin_echo.c\
               echo_utils.c\
               validate_commands.c\
               env_utils_b.c
               #builtins.c\
               #env_array.c\
               #utils_b.c\

SRCS        := $(addprefix $(SRC_DIR)/, $(SRC_FILES))


OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

