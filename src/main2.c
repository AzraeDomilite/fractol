#include "../include/fractol.h"
#include "../include/keys.h"
#include "../libs/libft/src/libft.h"
#include "../libs/ft_printf/include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Function prototypes

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define ZOOM_IN 1.1
#define ZOOM_OUT 0.9
#define MOVE_STEP 20
#define MAX_ITER 1000
#define MIN_ITER 10
#define FAST_ITER 100
#define ESCAPE_RADIUS 4.0
#define INITIAL_C_REAL -0.7
#define INITIAL_C_IMAG 0.27015
#define ITER_STEP 50
#define USAGE_MSG "Usage: ./fractol <julia|mandelbrot|tricorn>\n"
#define ERROR_MSG_WINDOW "Error\nWindow creation failed\n"
#define ERROR_MSG_IMAGE "Error\nImage creation failed\n"
#define ERROR_MSG_LOOP "Error\nProgram failed on loop\n"

typedef enum e_fractal_type {
	JULIA,
	MANDELBROT,
	TRICORN
} t_fractal_type;

typedef enum e_palette_type {
	PALETTE_DERIVATIVE_BAILOUT,
	PALETTE_STRIPES,
	PALETTE_SMOOTH,
	PALETTE_CLASSIC
} t_palette_type;

typedef struct s_data {
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	double			zoom;
	double			offset_x;
	double			offset_y;
	double			c_real;
	double			c_imag;
	int				redraw;
	int				fast_mode;
	int				max_iter;
	t_fractal_type	fractal_type;
	t_palette_type	palette_type;
} t_data;

void exit_fractol(t_data *data);
void reset_view(t_data *data);
void move_offset(t_data *data, double x_step, double y_step);
void adjust_fractal_parameters(int key, t_data *data);
void handle_reset_and_exit(int key, t_data *data);
void handle_movement(int key, t_data *data);
void handle_zoom(int key, t_data *data);
void handle_iter_adjustment(int key, t_data *data);
void switch_palette(t_data *data);
int deal_key(int key, t_data *data);
void put_pixel(t_data *data, int x, int y, int color);
int get_color(int iter, t_palette_type palette_type, double z_real, double z_imag);
int get_color_stripes(int iter);
int get_color_smooth(int iter);
int get_color_classic(int iter);
void draw_fractal(t_data *data);
int get_color_derivative_bailout(int iter, double z_real, double z_imag);
int render_next_frame(t_data *data);
int mouse_scroll(int button, int x, int y, t_data *data);
int main(int argc, char **argv);

void exit_fractol(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	exit(EXIT_SUCCESS);
}

void reset_view(t_data *data)
{
	data->zoom = 1.0;
	data->offset_x = 0;
	data->offset_y = 0;
	data->c_real = INITIAL_C_REAL;
	data->c_imag = INITIAL_C_IMAG;
	data->redraw = 1;
	data->fast_mode = 0;
	data->max_iter = 100;
	data->palette_type = PALETTE_STRIPES;
	ft_printf("Resetting view: c_real = %lf, c_imag = %lf\n", data->c_real, data->c_imag);
}

void move_offset(t_data *data, double x_step, double y_step)
{
	data->offset_x += x_step;
	data->offset_y += y_step;
	data->redraw = 1;
}

void adjust_fractal_parameters(int key, t_data *data)
{
	if (key == K_Z)
		switch_palette(data);
	if (key == K_O || key == K_I)
	{
		if (data->fractal_type == JULIA)
			data->c_imag += 0.01;
		else if (data->fractal_type == MANDELBROT || data->fractal_type == TRICORN)
			data->offset_y += (key == K_O) ? 0.01 : -0.01;
		data->redraw = 1;
	}
	else if (key == K_K || key == K_L)
	{
		if (data->fractal_type == JULIA)
			data->c_real += 0.01;
		else if (data->fractal_type == MANDELBROT || data->fractal_type == TRICORN)
			data->offset_x += (key == K_L) ? 0.01 : -0.01;
		data->redraw = 1;
	}
	else if (key == K_X)
	{
		data->palette_type = (data->palette_type + 1) % 4;
		data->redraw = 1;
	}
}

void handle_reset_and_exit(int key, t_data *data)
{
	if (key == K_R)
		reset_view(data);
	else if (key == K_ESC || key == K_Q)
		exit_fractol(data);
}

void handle_movement(int key, t_data *data)
{
	if (key == K_S || key == K_DOWN)
		move_offset(data, 0, MOVE_STEP);
	else if (key == K_W || key == K_UP)
		move_offset(data, 0, -MOVE_STEP);
	else if (key == K_D || key == K_RIGHT)
		move_offset(data, MOVE_STEP, 0);
	else if (key == K_A || key == K_LEFT)
		move_offset(data, -MOVE_STEP, 0);
}

void handle_zoom(int key, t_data *data)
{
	if (key == K_NUM_PLUS || key == K_NUM_MINUS)
	{
		data->zoom *= key == K_NUM_PLUS ? ZOOM_IN : ZOOM_OUT;
		data->redraw = 1;
	}
}

void handle_iter_adjustment(int key, t_data *data)
{
	if (key == K_F)
	{
		if (data->max_iter - ITER_STEP >= MIN_ITER)
			data->max_iter -= ITER_STEP;
		else
			data->max_iter = MIN_ITER;
		data->redraw = 1;
	}
	else if (key == K_G)
	{
		if (data->max_iter + ITER_STEP <= MAX_ITER)
			data->max_iter += ITER_STEP;
		else
			data->max_iter = MAX_ITER;
		data->redraw = 1;
	}
}

void switch_palette(t_data *data)
{
	data->palette_type = (data->palette_type + 1) % 4;
	data->redraw = 1;
}

int deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);

	data->fast_mode = 1;
	ft_printf("Key pressed: %d, max_iter: %d, palette_type: %d\n", key, data->max_iter, data->palette_type);
	return 0;
}

void put_pixel(t_data *data, int x, int y, int color)
{
	char *dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

int get_color(int iter, t_palette_type palette_type, double z_real, double z_imag)
{
	if (palette_type == PALETTE_STRIPES)
		return get_color_stripes(iter);
	else if (palette_type == PALETTE_SMOOTH)
		return get_color_smooth(iter);
	else if (palette_type == PALETTE_CLASSIC)
		return get_color_classic(iter);
	else if (palette_type == PALETTE_DERIVATIVE_BAILOUT)
		return get_color_derivative_bailout(iter, z_real, z_imag);
	return 0;
}

int get_color_derivative_bailout(int iter, double z_real, double z_imag)
{
	double d = sqrt(z_real * z_real + z_imag * z_imag);
	double t = iter - log2(log2(d));
	int r = (int)(255 * fabs(sin(t)));
	int g = (int)(255 * fabs(sin(t + 2)));
	int b = (int)(255 * fabs(sin(t + 4)));
	return ((r << 16) | (g << 8) | b);
}

int get_color_stripes(int iter) {
	double t = 0.5 * (1 + sin(iter * 0.1));
	int r = (int)(9 * (1 - t) * t * t * t * 255);
	int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((r << 16) | (g << 8) | b);
}

int get_color_smooth(int iter)
{
	double t = (double)iter / MAX_ITER;
	int r = (int)(255 * t);
	int g = (int)(255 * (1 - t));
	int b = (int)(255 * (t * (1 - t)));
	return ((r << 16) | (g << 8) | b);
}

int get_color_classic(int iter)
{
	int r = (iter * 9) % 256;
	int g = (iter * 7) % 256;
	int b = (iter * 5) % 256;
	return ((r << 16) | (g << 8) | b);
}

void draw_fractal(t_data *data)
{
	int x, y, iter;
	double c_real, c_imag, z_real, z_imag, tmp;
	double scale = 4.0 / (WIN_WIDTH * data->zoom);
	double real_min = (-WIN_WIDTH / 2.0 + data->offset_x) * scale;
	double imag_min = (-WIN_HEIGHT / 2.0 + data->offset_y) * scale;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			if (data->fractal_type == JULIA)
			{
				z_real = real_min + x * scale;
				z_imag = imag_min + y * scale;
				iter = 0;
				while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS && iter < data->max_iter)
				{
					tmp = z_real * z_real - z_imag * z_imag + data->c_real;
					z_imag = 2.0 * z_real * z_imag + data->c_imag;
					z_real = tmp;
					iter++;
				}
			}
			else if (data->fractal_type == MANDELBROT || data->fractal_type == TRICORN)
			{
				c_real = real_min + x * scale;
				c_imag = imag_min + y * scale;
				z_real = 0;
				z_imag = 0;
				iter = 0;
				while (z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS && iter < data->max_iter)
				{
					tmp = z_real * z_real - z_imag * z_imag + c_real;
					if (data->fractal_type == TRICORN)
						z_imag = -2.0 * z_real * z_imag + c_imag;
					else
						z_imag = 2.0 * z_real * z_imag + c_imag;
					z_real = tmp;
					iter++;
				}
			}
			put_pixel(data, x, y, get_color(iter, data->palette_type, z_real, z_imag));
			x++;
		}
		y++;
	}
}

int render_next_frame(t_data *data)
{
	char info[100];
	if (!data->redraw)
		return (0);

	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	draw_fractal(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);

	snprintf(info, sizeof(info), "c_real: %lf", data->c_real);
	mlx_string_put(data->mlx, data->win, 10, 10, 0xFFFFFF, info);
	snprintf(info, sizeof(info), "c_imag: %lf", data->c_imag);
	mlx_string_put(data->mlx, data->win, 10, 30, 0xFFFFFF, info);
	snprintf(info, sizeof(info), "max_iter: %d", data->max_iter);
	mlx_string_put(data->mlx, data->win, 10, 50, 0xFFFFFF, info);
	const char *palette_names[] = {"Stripes", "Smooth", "Classic", "Derivative Bailout"};
	snprintf(info, sizeof(info), "palette: %s", palette_names[data->palette_type]);
	mlx_string_put(data->mlx, data->win, 10, 70, 0xFFFFFF, info);

	data->fast_mode = 0;
	data->redraw = 0;
	return (0);
}

int mouse_scroll(int button, int x, int y, t_data *data)
{
	double prev_zoom;

	prev_zoom = data->zoom;
	if (button == 4)
		data->zoom *= ZOOM_IN;
	else if (button == 5)
		data->zoom *= ZOOM_OUT;

	data->offset_x = (WIN_WIDTH / 2 - x) * (1 - data->zoom / prev_zoom) + data->offset_x;
	data->offset_y = (WIN_HEIGHT / 2 - y) * (1 - data->zoom / prev_zoom) + data->offset_y;

	data->redraw = 1;
	data->fast_mode = 1;
	return (0);
}

int main(int argc, char **argv)
{
	t_data data;

	if (argc != 2)
	{
		ft_printf(USAGE_MSG);
		return (1);
	}

	data.mlx = mlx_init();
	if (!data.mlx)
	{
		ft_printf("Error\nFailed to initialize MLX\n");
		return (EXIT_FAILURE);
	}
	reset_view(&data);
	data.img = NULL;

	if (ft_strcmp(argv[1], "julia") == 0)
		data.fractal_type = JULIA;
	else if (ft_strcmp(argv[1], "mandelbrot") == 0)
		data.fractal_type = MANDELBROT;
	else if (ft_strcmp(argv[1], "tricorn") == 0)
		data.fractal_type = TRICORN;
	else
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}

	data.win = mlx_new_window(data.mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data.win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	mlx_key_hook(data.win, deal_key, &data);
	mlx_hook(data.win, 17, 0, (int (*)())exit_fractol, &data);
	mlx_hook(data.win, 4, 0, mouse_scroll, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);

	ft_printf(ERROR_MSG_LOOP);
	exit(EXIT_FAILURE);
}
