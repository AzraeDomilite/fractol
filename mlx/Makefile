NOM=libmlx.a
SRC= mlx_shaders.c mlx_new_window.m mlx_init_loop.m mlx_new_image.m mlx_xpm.c mlx_int_str_to_wordtab.c
SRC+= mlx_png.c mlx_mouse.m
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.m=.o)
CFLAGS+=-O2 -DGL_SILENCE_DEPRECATION -DSTRINGPUTX11

GREEN = \033[1;32m
RESET = \033[0m

all: $(NOM)

$(NOM): $(OBJ)
	@ar -r $(NOM) $(OBJ) > /dev/null 2>&1
	@ranlib $(NOM) > /dev/null 2>&1
	@echo "$(GREEN)Compilation of MLX successful! 😊$(RESET)"

%.o: %.c
	@cc $(CFLAGS) -c -o $@ $< > /dev/null 2>&1

%.o: %.m
	@cc $(CFLAGS) -c -o $@ $< > /dev/null 2>&1

clean:
	@rm -f $(NOM) $(OBJ) *~ > /dev/null 2>&1
	@rm -f mlx_app > /dev/null 2>&1

fclean: clean
	@rm -f $(NOM) > /dev/null 2>&1

re: fclean all
