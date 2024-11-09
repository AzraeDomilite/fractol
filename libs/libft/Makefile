NAME = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SRC_FILES = ft_atoi.c ft_isalpha.c ft_itoa.c ft_lstdelone.c ft_lstnew.c ft_memcpy.c \
            ft_putendl_fd.c ft_strchr.c ft_strlcat.c ft_strncmp.c ft_substr.c ft_bzero.c \
            ft_isascii.c ft_lstadd_back.c ft_lstiter.c ft_lstsize.c ft_memmove.c ft_putnbr_fd.c \
            ft_strdup.c ft_strlcpy.c ft_strnstr.c ft_tolower.c ft_calloc.c ft_isdigit.c \
            ft_lstadd_front.c ft_lstlast.c ft_memchr.c ft_memset.c ft_putstr_fd.c \
            ft_striteri.c ft_strlen.c ft_strrchr.c ft_toupper.c ft_putchar_fd.c ft_split.c ft_strjoin.c

OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

GREEN = \033[1;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@ar rcs $(NAME) $(OBJ_FILES)
	@echo "$(GREEN)Compilation of libft successful!     😊$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -o $@ -c $<

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
