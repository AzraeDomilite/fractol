NAME = fractol
OS = $(shell uname)

CC = gcc
CFLAGS =  -Wall -Wextra -pthread -I./include

LIBS_DIR = ./libs
OBJ_DIR = ./obj
SRC_DIR = ./src
PRINTF_DIR = $(LIBS_DIR)/ft_printf
LIBFT_DIR = $(LIBS_DIR)/libft
MLX = ./mlx

LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

ifeq ($(OS), Linux)
	MLX_LNK = -L $(MLX) -lmlx -lXext -lX11
else
	MLX_LNK = -L $(MLX) -lmlx -framework OpenGL -framework AppKit
endif

MLX_INC = -I $(MLX)
MLX_LIB = $(MLX)/mlx.a

GREEN = \033[1;32m
RESET = \033[0m

all: $(LIBFT) $(PRINTF) $(MLX_LIB) $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT) $(PRINTF) $(MLX_LNK)
	@echo "$(GREEN)Compilation of fractol successful!   😊$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(MLX_INC) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(PRINTF):
	@$(MAKE) -C $(PRINTF_DIR)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(MLX_LIB):
	@$(MAKE) -C $(MLX)
	
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(PRINTF_DIR) clean
	@$(MAKE) -C $(MLX) clean || rm -f $(MLX)/*.o $(MLX)/*.swiftdoc $(MLX)/*.swiftmodule $(MLX)/*.dylib

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(PRINTF_DIR) fclean
	@$(MAKE) -C $(MLX) fclean
	@echo "$(GREEN)Clean done 🧹$(RESET)"

re: fclean all

.PHONY: all clean fclean re
