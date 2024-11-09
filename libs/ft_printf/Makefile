NAME = libftprintf.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SRC_FILES = ft_printf.c ft_printf_utils_1.c ft_printf_utils_2.c

OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

GREEN = \033[1;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@ar rcs $(NAME) $(OBJ_FILES)
	@echo "$(GREEN)Compilation of ft_printf successful! 😊$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $@ -c $<

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
