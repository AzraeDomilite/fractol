/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:30:45 by blucken           #+#    #+#             */
/*   Updated: 2024/11/18 17:30:45 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"
#include "../include/keys.h"
#include "../libs/libft/src/libft.h"
#include "../libs/ft_printf/include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define ZOOM_IN 1.1
#define ZOOM_OUT 0.9
#define MOVE_STEP 20
#define MAX_ITER 1000
#define MIN_ITER 10
#define DEFAULT_PREVIEW_ITER 100
#define ESCAPE_RADIUS 4.0
#define INITIAL_C_REAL -0.7
#define INITIAL_C_IMAG 0.27015
#define ITER_STEP 50
#define USAGE_MSG "Usage: ./fractol <julia|mandelbrot|tricorn|burningship|lyapunov|newton>\n"
#define ERROR_MSG_WINDOW "Error\nWindow creation failed\n"
#define ERROR_MSG_IMAGE "Error\nImage creation failed\n"
#define ERROR_MSG_LOOP "Error\nProgram failed on loop\n"
#define PALETTE_COUNT 15
#define LYAPUNOV_SEQUENCE "AB"

typedef enum e_fractal_type {
	JULIA,
	MANDELBROT,
	TRICORN,
	BURNING_SHIP,
	LYAPUNOV,
	NEWTON
}	t_fractal_type;

typedef enum e_palette_type {
	PALETTE_DERIVATIVE_BAILOUT,
	PALETTE_STRIPES,
	PALETTE_SMOOTH,
	PALETTE_CLASSIC,
	PALETTE_LOGARITHMIC,
	PALETTE_HSV,
	PALETTE_GRADIENT,
	PALETTE_BLACK_WHITE,
	PALETTE_ESCAPE_TIME,
	PALETTE_CONTINUOUS_POTENTIAL,
	PALETTE_FIRE,
	PALETTE_INTERIOR_DISTANCE,
	PALETTE_LCH,
	PALETTE_EXP_CYCLIC_LCH_NO_SHADING,
	PALETTE_EXP_CYCLIC_LCH_SHADING
}	t_palette_type;

typedef struct s_color {
	int	r;
	int	g;
	int	b;
}	t_color;

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
	int				is_selecting;
	int				select_start_x;
	int				select_start_y;
	int				select_end_x;
	int				select_end_y;
	t_fractal_type	fractal_type;
	t_palette_type	palette_type;
	t_color			base_color;
}	t_data;

typedef struct s_fractal_vars {
	int		x;
	int		iter;
	double	c_real;
	double	c_imag;
	double	z_real;
	double	z_imag;
}	t_fractal_vars;

typedef struct s_color_vars {
	double	t;
	double	l;
	double	c;
	double	h;
	int		r;
	int		g;
	int		b;
}	t_color_vars;

/* Function prototypes */

void	parse_arguments(int argc, char **argv, t_data *data);
void	initialize_mlx(t_data *data);
void	reset_view(t_data *data);
void	exit_fractol(t_data *data);
int		deal_key(int key, t_data *data);
void	adjust_fractal_parameters(int key, t_data *data);
void	handle_reset_and_exit(int key, t_data *data);
void	handle_movement(int key, t_data *data);
void	move_offset(t_data *data, double x_step, double y_step);
void	handle_zoom(int key, t_data *data);
void	handle_iter_adjustment(int key, t_data *data);
void	switch_palette_next(t_data *data);
void	switch_palette_prev(t_data *data);
void	adjust_c_imag(t_data *data, double delta);
void	adjust_c_real(t_data *data, double delta);
void	adjust_base_color_component(t_data *data, int key);
void	reset_base_color_component(t_data *data, int key);
int		mouse_press(int button, int x, int y, t_data *data);
int		mouse_move(int x, int y, t_data *data);
int		mouse_release(int button, int x, int y, t_data *data);
int		render_next_frame(t_data *data);
void	draw_fractal_with_iter(t_data *data, int iter_count);
void	draw_fractal(t_data *data, int iter_count);
void	draw_fractal_line(t_data *data, int y,
			double real_min, double imag_min, double scale, int iter_count);
void	put_pixel(t_data *data, int x, int y, int color);
void	draw_info_strings(t_data *data);
void	draw_zoom_level(t_data *data);
void	draw_selection_rectangle(t_data *data);
void	get_sorted_selection(t_data *data, int *x_start, int *x_end,
			int *y_start, int *y_end);
void	draw_rectangle_edges(t_data *data, int x_start, int y_start,
			int x_end, int y_end);
void	zoom_to_selection(t_data *data);
void	calculate_zoom_and_offset(t_data *data, int x_start, int x_end,
			int y_start, int y_end);
void	update_zoom_and_offset(t_data *data, double x_min, double x_max,
			double y_min, double y_max);
void	swap_int(int *a, int *b);
int		compute_julia(t_data *data, double z_real, double z_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_mandelbrot(t_data *data, double c_real, double c_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_burning_ship(double c_real, double c_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_lyapunov(double x, double y, int iter_count);
int		compute_newton(double z_real, double z_imag, int iter_count);
int		get_color(int iter, t_data *data,
			double z_real, double z_imag, int max_iter);
int		get_color_stripes(int iter, t_data *data);
int		get_color_smooth(int iter, t_data *data);
int		get_color_classic(int iter, t_data *data);
int		get_color_logarithmic(int iter, int max_iter, t_data *data);
int		get_color_hsv(int iter, int max_iter, t_data *data);
int		get_color_gradient(int iter, int max_iter, t_data *data);
int		get_color_black_white(int iter, t_data *data);
int		get_color_escape_time(int iter, int max_iter, t_data *data);
int		get_color_continuous_potential(int iter, double z_real,
			double z_imag, int max_iter, t_data *data);
int		get_color_fire(int iter, int max_iter, t_data *data);
int		get_color_derivative_bailout(int iter,
			double z_real, double z_imag, t_data *data);
int		get_color_interior_distance(int iter, double z_real,
			double z_imag, int max_iter, t_data *data);
int		get_color_lch(int iter, int max_iter, t_data *data);
int		get_color_exp_cyclic_lch_no_shading(int iter, int max_iter,
			t_data *data);
int		get_color_exp_cyclic_lch_shading(int iter, int max_iter,
			t_data *data);
void	lch_to_rgb(double l, double c, double h_deg, int *r, int *g, int *b);
void	lch_to_lab(double l, double c, double h_deg,
			double *a, double *b_lab);
void	lab_to_xyz(double l, double a, double b_lab,
			double *x, double *y, double *z);
void	xyz_to_rgb(double x, double y, double z, int *r, int *g, int *b);
char	*ft_ftoa(double n, int precision);
char	*ft_lltoa(long long n);
static int	ft_numlen(long long n);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1);

/* Main function */

int	main(int argc, char **argv)
{
	t_data	data;

	parse_arguments(argc, argv, &data);
	initialize_mlx(&data);
	reset_view(&data);
	mlx_key_hook(data.win, deal_key, &data);
	mlx_hook(data.win, 17, 0, (int (*)())exit_fractol, &data);
	mlx_hook(data.win, 4, 0, mouse_press, &data);
	mlx_hook(data.win, 5, 0, mouse_release, &data);
	mlx_hook(data.win, 6, 0, mouse_move, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);
	ft_printf(ERROR_MSG_LOOP);
	exit(EXIT_FAILURE);
}

/* Function implementations */

/* Parse command line arguments */
void	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "julia") == 0)
		data->fractal_type = JULIA;
	else if (ft_strcmp(argv[1], "mandelbrot") == 0)
		data->fractal_type = MANDELBROT;
	else if (ft_strcmp(argv[1], "tricorn") == 0)
		data->fractal_type = TRICORN;
	else if (ft_strcmp(argv[1], "burningship") == 0)
		data->fractal_type = BURNING_SHIP;
	else if (ft_strcmp(argv[1], "lyapunov") == 0)
		data->fractal_type = LYAPUNOV;
	else if (ft_strcmp(argv[1], "newton") == 0)
		data->fractal_type = NEWTON;
	else
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
}

/* Initialize MLX and create window */
void	initialize_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		ft_printf("Error\nFailed to initialize MLX\n");
		exit(EXIT_FAILURE);
	}
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = NULL;
}

/* Reset view to initial parameters */
void	reset_view(t_data *data)
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
	data->base_color.r = 255;
	data->base_color.g = 255;
	data->base_color.b = 255;
	ft_printf("Resetting view: c_real = %lf, c_imag = %lf\n",
		data->c_real, data->c_imag);
}

/* Exit the program and free resources */
void	exit_fractol(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	exit(EXIT_SUCCESS);
}

/* Handle key events */
int	deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);
	data->fast_mode = 1;
	ft_printf("Key pressed: %d, max_iter: %d, palette_type: %d\n",
		key, data->max_iter, data->palette_type);
	return (0);
}

/* Adjust fractal parameters based on key input */
void	adjust_fractal_parameters(int key, t_data *data)
{
	if (key == K_Z)
		switch_palette_prev(data);
	else if (key == K_X)
		switch_palette_next(data);
	else if (key == K_O)
		adjust_c_imag(data, 0.01);
	else if (key == K_I)
		adjust_c_imag(data, -0.01);
	else if (key == K_K)
		adjust_c_real(data, 0.01);
	else if (key == K_L)
		adjust_c_real(data, -0.01);
	else if (key == K_NUM_7 || key == K_NUM_9
		|| key == K_NUM_4 || key == K_NUM_6
		|| key == K_NUM_1 || key == K_NUM_3)
		adjust_base_color_component(data, key);
	else if (key == K_NUM_8 || key == K_NUM_5 || key == K_NUM_2)
		reset_base_color_component(data, key);
}

/* Handle reset and exit keys */
void	handle_reset_and_exit(int key, t_data *data)
{
	if (key == K_R)
		reset_view(data);
	else if (key == K_ESC || key == K_Q)
		exit_fractol(data);
}

/* Handle movement keys */
void	handle_movement(int key, t_data *data)
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

/* Move the view offset */
void	move_offset(t_data *data, double x_step, double y_step)
{
	data->offset_x += x_step;
	data->offset_y += y_step;
	data->redraw = 1;
}

/* Handle zoom keys */
void	handle_zoom(int key, t_data *data)
{
	if (key == K_NUM_PLUS)
	{
		data->zoom *= ZOOM_IN;
		data->redraw = 1;
	}
	else if (key == K_NUM_MINUS)
	{
		data->zoom *= ZOOM_OUT;
		data->redraw = 1;
	}
}

/* Handle iteration adjustment keys */
void	handle_iter_adjustment(int key, t_data *data)
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

/* Switch to the next color palette */
void	switch_palette_next(t_data *data)
{
	data->palette_type = (data->palette_type + 1) % PALETTE_COUNT;
	data->redraw = 1;
}

/* Switch to the previous color palette */
void	switch_palette_prev(t_data *data)
{
	if (data->palette_type == 0)
		data->palette_type = PALETTE_COUNT - 1;
	else
		data->palette_type--;
	data->redraw = 1;
}

/* Adjust imaginary part of c */
void	adjust_c_imag(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_imag += delta;
	else
		data->offset_y += delta;
	data->redraw = 1;
}

/* Adjust real part of c */
void	adjust_c_real(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_real += delta;
	else
		data->offset_x += delta;
	data->redraw = 1;
}

/* Adjust base color component */
void	adjust_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_7)
	{
		if (data->base_color.r - 15 < 0)
			data->base_color.r = 0;
		else
			data->base_color.r -= 15;
	}
	else if (key == K_NUM_9)
	{
		if (data->base_color.r + 15 > 255)
			data->base_color.r = 255;
		else
			data->base_color.r += 15;
	}
	else if (key == K_NUM_4)
	{
		if (data->base_color.g - 15 < 0)
			data->base_color.g = 0;
		else
			data->base_color.g -= 15;
	}
	else if (key == K_NUM_6)
	{
		if (data->base_color.g + 15 > 255)
			data->base_color.g = 255;
		else
			data->base_color.g += 15;
	}
	else if (key == K_NUM_1)
	{
		if (data->base_color.b - 15 < 0)
			data->base_color.b = 0;
		else
			data->base_color.b -= 15;
	}
	else if (key == K_NUM_3)
	{
		if (data->base_color.b + 15 > 255)
			data->base_color.b = 255;
		else
			data->base_color.b += 15;
	}
	data->redraw = 1;
}

/* Reset base color component */
void	reset_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_8)
		data->base_color.r = 255;
	else if (key == K_NUM_5)
		data->base_color.g = 255;
	else if (key == K_NUM_2)
		data->base_color.b = 255;
	data->redraw = 1;
}

/* Mouse press event */
int	mouse_press(int button, int x, int y, t_data *data)
{
	if (button == 1)
	{
		data->is_selecting = 1;
		data->select_start_x = x;
		data->select_start_y = y;
		data->select_end_x = x;
		data->select_end_y = y;
	}
	return (0);
}

/* Mouse move event */
int	mouse_move(int x, int y, t_data *data)
{
	if (data->is_selecting)
	{
		data->select_end_x = x;
		data->select_end_y = y;
		data->redraw = 1;
	}
	return (0);
}

/* Mouse release event */
int	mouse_release(int button, int x, int y, t_data *data)
{
	if (button == 1 && data->is_selecting)
	{
		data->is_selecting = 0;
		data->select_end_x = x;
		data->select_end_y = y;
		zoom_to_selection(data);
	}
	return (0);
}

/* Render the next frame */
int	render_next_frame(t_data *data)
{
	if (!data->redraw)
		return (0);
	if (data->fast_mode)
		draw_fractal_with_iter(data, DEFAULT_PREVIEW_ITER);
	else
		draw_fractal_with_iter(data, data->max_iter);
	data->fast_mode = 0;
	data->redraw = 0;
	return (0);
}

/* Draw the fractal with a given iteration count */
void	draw_fractal_with_iter(t_data *data, int iter_count)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	draw_fractal(data, iter_count);
	if (data->is_selecting)
		draw_selection_rectangle(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	draw_info_strings(data);
}

/* Draw the fractal */
void	draw_fractal(t_data *data, int iter_count)
{
	int		y;
	double	scale;
	double	real_min;
	double	imag_min;

	scale = 4.0 / (WIN_WIDTH * data->zoom);
	real_min = data->offset_x - (WIN_WIDTH / 2.0) * scale;
	imag_min = data->offset_y - (WIN_HEIGHT / 2.0) * scale;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		draw_fractal_line(data, y, real_min, imag_min, scale, iter_count);
		y++;
	}
}

/* Draw a line of the fractal */
void	draw_fractal_line(t_data *data, int y,
	double real_min, double imag_min, double scale, int iter_count)
{
	t_fractal_vars	vars;

	vars.x = 0;
	while (vars.x < WIN_WIDTH)
	{
		vars.c_real = real_min + vars.x * scale;
		vars.c_imag = imag_min + y * scale;
		if (data->fractal_type == JULIA)
		{
			vars.z_real = vars.c_real;
			vars.z_imag = vars.c_imag;
			vars.iter = compute_julia(data, vars.z_real, vars.z_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == MANDELBROT
			|| data->fractal_type == TRICORN)
		{
			vars.iter = compute_mandelbrot(data, vars.c_real, vars.c_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == BURNING_SHIP)
		{
			vars.iter = compute_burning_ship(vars.c_real, vars.c_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == LYAPUNOV)
		{
			vars.iter = compute_lyapunov(vars.c_real, vars.c_imag, iter_count);
		}
		else if (data->fractal_type == NEWTON)
		{
			vars.iter = compute_newton(vars.c_real, vars.c_imag, iter_count);
		}
		put_pixel(data, vars.x, y, get_color(vars.iter,
				data, vars.z_real, vars.z_imag, data->max_iter));
		vars.x++;
	}
}

/* Draw information strings on the window */
void	draw_info_strings(t_data *data)
{
	const char	*palette_names[PALETTE_COUNT] = {
		"Derivative Bailout", "Stripes", "Smooth", "Classic",
		"Logarithmic", "HSV", "Gradient", "Black & White",
		"Escape Time", "Continuous Potential", "Fire",
		"Interior Distance", "LCH Coloring",
		"Exp Cyclic LCH No Shading", "Exp Cyclic LCH Shading"
	};
	mlx_string_put(data->mlx, data->win, 10, 10, 0xFFFFFF,
		"Fract'ol - 42 Project");
	mlx_string_put(data->mlx, data->win, 10, 30, 0xFFFFFF,
		"Press R to reset view");
	mlx_string_put(data->mlx, data->win, 10, 50, 0xFFFFFF,
		"Use arrow keys or WASD to move");
	mlx_string_put(data->mlx, data->win, 10, 70, 0xFFFFFF,
		"Use + and - to zoom");
	mlx_string_put(data->mlx, data->win, 10, 90, 0xFFFFFF,
		"Palette: ");
	mlx_string_put(data->mlx, data->win, 100, 90, 0xFFFFFF,
		(char *)palette_names[data->palette_type]);
	draw_zoom_level(data);
}

/* Draw the zoom level */
void	draw_zoom_level(t_data *data)
{
	char	*str;
	char	*temp;

	temp = ft_ftoa(data->zoom, 6);
	str = ft_strjoin("Zoom: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, 110, 0xFFFFFF, str);
	free(str);
}

/* Draw the selection rectangle */
void	draw_selection_rectangle(t_data *data)
{
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	draw_rectangle_edges(data, x_start, y_start, x_end, y_end);
}

/* Get sorted selection coordinates */
void	get_sorted_selection(t_data *data, int *x_start, int *x_end,
	int *y_start, int *y_end)
{
	*x_start = data->select_start_x;
	*y_start = data->select_start_y;
	*x_end = data->select_end_x;
	*y_end = data->select_end_y;
	if (*x_start > *x_end)
		swap_int(x_start, x_end);
	if (*y_start > *y_end)
		swap_int(y_start, y_end);
}

/* Draw the edges of the rectangle */
void	draw_rectangle_edges(t_data *data, int x_start, int y_start,
	int x_end, int y_end)
{
	int	x;
	int	y;

	x = x_start;
	while (x <= x_end)
	{
		put_pixel(data, x, y_start, 0xFFFFFF);
		put_pixel(data, x, y_end, 0xFFFFFF);
		x++;
	}
	y = y_start;
	while (y <= y_end)
	{
		put_pixel(data, x_start, y, 0xFFFFFF);
		put_pixel(data, x_end, y, 0xFFFFFF);
		y++;
	}
}

/* Put a pixel on the image */
void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

/* Swap two integers */
void	swap_int(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourusername <yourusername@student.42.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 10:00:00 by yourusername      #+#    #+#             */
/*   Updated: 2023/10/17 12:00:00 by yourusername     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"
#include "../include/keys.h"
#include "../libs/libft/src/libft.h"
#include "../libs/ft_printf/include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define ZOOM_IN 1.1
#define ZOOM_OUT 0.9
#define MOVE_STEP 20
#define MAX_ITER 1000
#define MIN_ITER 10
#define DEFAULT_PREVIEW_ITER 100
#define ESCAPE_RADIUS 4.0
#define INITIAL_C_REAL -0.7
#define INITIAL_C_IMAG 0.27015
#define ITER_STEP 50
#define USAGE_MSG "Usage: ./fractol <julia|mandelbrot|tricorn|burningship|lyapunov|newton>\n"
#define ERROR_MSG_WINDOW "Error\nWindow creation failed\n"
#define ERROR_MSG_IMAGE "Error\nImage creation failed\n"
#define ERROR_MSG_LOOP "Error\nProgram failed on loop\n"
#define PALETTE_COUNT 15
#define LYAPUNOV_SEQUENCE "AB"

typedef enum e_fractal_type {
	JULIA,
	MANDELBROT,
	TRICORN,
	BURNING_SHIP,
	LYAPUNOV,
	NEWTON
}	t_fractal_type;

typedef enum e_palette_type {
	PALETTE_DERIVATIVE_BAILOUT,
	PALETTE_STRIPES,
	PALETTE_SMOOTH,
	PALETTE_CLASSIC,
	PALETTE_LOGARITHMIC,
	PALETTE_HSV,
	PALETTE_GRADIENT,
	PALETTE_BLACK_WHITE,
	PALETTE_ESCAPE_TIME,
	PALETTE_CONTINUOUS_POTENTIAL,
	PALETTE_FIRE,
	PALETTE_INTERIOR_DISTANCE,
	PALETTE_LCH,
	PALETTE_EXP_CYCLIC_LCH_NO_SHADING,
	PALETTE_EXP_CYCLIC_LCH_SHADING
}	t_palette_type;

typedef struct s_color {
	int	r;
	int	g;
	int	b;
}	t_color;

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
	int				is_selecting;
	int				select_start_x;
	int				select_start_y;
	int				select_end_x;
	int				select_end_y;
	t_fractal_type	fractal_type;
	t_palette_type	palette_type;
	t_color			base_color;
}	t_data;

typedef struct s_fractal_vars {
	int		x;
	int		iter;
	double	c_real;
	double	c_imag;
	double	z_real;
	double	z_imag;
}	t_fractal_vars;

typedef struct s_color_vars {
	double	t;
	double	l;
	double	c;
	double	h;
	int		r;
	int		g;
	int		b;
}	t_color_vars;

/* Function prototypes */

void	parse_arguments(int argc, char **argv, t_data *data);
void	initialize_mlx(t_data *data);
void	reset_view(t_data *data);
void	exit_fractol(t_data *data);
int		deal_key(int key, t_data *data);
void	adjust_fractal_parameters(int key, t_data *data);
void	handle_reset_and_exit(int key, t_data *data);
void	handle_movement(int key, t_data *data);
void	move_offset(t_data *data, double x_step, double y_step);
void	handle_zoom(int key, t_data *data);
void	handle_iter_adjustment(int key, t_data *data);
void	switch_palette_next(t_data *data);
void	switch_palette_prev(t_data *data);
void	adjust_c_imag(t_data *data, double delta);
void	adjust_c_real(t_data *data, double delta);
void	adjust_base_color_component(t_data *data, int key);
void	reset_base_color_component(t_data *data, int key);
int		mouse_press(int button, int x, int y, t_data *data);
int		mouse_move(int x, int y, t_data *data);
int		mouse_release(int button, int x, int y, t_data *data);
int		render_next_frame(t_data *data);
void	draw_fractal_with_iter(t_data *data, int iter_count);
void	draw_fractal(t_data *data, int iter_count);
void	draw_fractal_line(t_data *data, int y,
			double real_min, double imag_min, double scale, int iter_count);
void	put_pixel(t_data *data, int x, int y, int color);
void	draw_info_strings(t_data *data);
void	draw_zoom_level(t_data *data);
void	draw_selection_rectangle(t_data *data);
void	get_sorted_selection(t_data *data, int *x_start, int *x_end,
			int *y_start, int *y_end);
void	draw_rectangle_edges(t_data *data, int x_start, int y_start,
			int x_end, int y_end);
void	zoom_to_selection(t_data *data);
void	calculate_zoom_and_offset(t_data *data, int x_start, int x_end,
			int y_start, int y_end);
void	update_zoom_and_offset(t_data *data, double x_min, double x_max,
			double y_min, double y_max);
void	swap_int(int *a, int *b);
int		compute_julia(t_data *data, double z_real, double z_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_mandelbrot(t_data *data, double c_real, double c_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_burning_ship(double c_real, double c_imag,
			double *final_z_real, double *final_z_imag, int iter_count);
int		compute_lyapunov(double x, double y, int iter_count);
int		compute_newton(double z_real, double z_imag, int iter_count);
int		get_color(int iter, t_data *data,
			double z_real, double z_imag, int max_iter);
int		get_color_stripes(int iter, t_data *data);
int		get_color_smooth(int iter, t_data *data);
int		get_color_classic(int iter, t_data *data);
int		get_color_logarithmic(int iter, int max_iter, t_data *data);
int		get_color_hsv(int iter, int max_iter, t_data *data);
int		get_color_gradient(int iter, int max_iter, t_data *data);
int		get_color_black_white(int iter, t_data *data);
int		get_color_escape_time(int iter, int max_iter, t_data *data);
int		get_color_continuous_potential(int iter, double z_real,
			double z_imag, int max_iter, t_data *data);
int		get_color_fire(int iter, int max_iter, t_data *data);
int		get_color_derivative_bailout(int iter,
			double z_real, double z_imag, t_data *data);
int		get_color_interior_distance(int iter, double z_real,
			double z_imag, int max_iter, t_data *data);
int		get_color_lch(int iter, int max_iter, t_data *data);
int		get_color_exp_cyclic_lch_no_shading(int iter, int max_iter,
			t_data *data);
int		get_color_exp_cyclic_lch_shading(int iter, int max_iter,
			t_data *data);
void	lch_to_rgb(double l, double c, double h_deg, int *r, int *g, int *b);
void	lch_to_lab(double l, double c, double h_deg,
			double *a, double *b_lab);
void	lab_to_xyz(double l, double a, double b_lab,
			double *x, double *y, double *z);
void	xyz_to_rgb(double x, double y, double z, int *r, int *g, int *b);
void	hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b);
char	*ft_ftoa(double n, int precision);
char	*ft_lltoa(long long n);
static int	ft_numlen(long long n);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1);

/* Main function */

int	main(int argc, char **argv)
{
	t_data	data;

	parse_arguments(argc, argv, &data);
	initialize_mlx(&data);
	reset_view(&data);
	mlx_key_hook(data.win, deal_key, &data);
	mlx_hook(data.win, 17, 0, (int (*)())exit_fractol, &data);
	mlx_hook(data.win, 4, 0, mouse_press, &data);
	mlx_hook(data.win, 5, 0, mouse_release, &data);
	mlx_hook(data.win, 6, 0, mouse_move, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);
	ft_printf(ERROR_MSG_LOOP);
	exit(EXIT_FAILURE);
}

/* Function implementations */

/* Parse command line arguments */
void	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "julia") == 0)
		data->fractal_type = JULIA;
	else if (ft_strcmp(argv[1], "mandelbrot") == 0)
		data->fractal_type = MANDELBROT;
	else if (ft_strcmp(argv[1], "tricorn") == 0)
		data->fractal_type = TRICORN;
	else if (ft_strcmp(argv[1], "burningship") == 0)
		data->fractal_type = BURNING_SHIP;
	else if (ft_strcmp(argv[1], "lyapunov") == 0)
		data->fractal_type = LYAPUNOV;
	else if (ft_strcmp(argv[1], "newton") == 0)
		data->fractal_type = NEWTON;
	else
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
}

/* Initialize MLX and create window */
void	initialize_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		ft_printf("Error\nFailed to initialize MLX\n");
		exit(EXIT_FAILURE);
	}
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = NULL;
}

/* Reset view to initial parameters */
void	reset_view(t_data *data)
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
	data->base_color.r = 255;
	data->base_color.g = 255;
	data->base_color.b = 255;
	ft_printf("Resetting view: c_real = %lf, c_imag = %lf\n",
		data->c_real, data->c_imag);
}

/* Exit the program and free resources */
void	exit_fractol(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	exit(EXIT_SUCCESS);
}

/* Handle key events */
int	deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);
	data->fast_mode = 1;
	ft_printf("Key pressed: %d, max_iter: %d, palette_type: %d\n",
		key, data->max_iter, data->palette_type);
	return (0);
}

/* Adjust fractal parameters based on key input */
void	adjust_fractal_parameters(int key, t_data *data)
{
	if (key == K_Z)
		switch_palette_prev(data);
	else if (key == K_X)
		switch_palette_next(data);
	else if (key == K_O)
		adjust_c_imag(data, 0.01);
	else if (key == K_I)
		adjust_c_imag(data, -0.01);
	else if (key == K_K)
		adjust_c_real(data, 0.01);
	else if (key == K_L)
		adjust_c_real(data, -0.01);
	else if (key == K_NUM_7 || key == K_NUM_9
		|| key == K_NUM_4 || key == K_NUM_6
		|| key == K_NUM_1 || key == K_NUM_3)
		adjust_base_color_component(data, key);
	else if (key == K_NUM_8 || key == K_NUM_5 || key == K_NUM_2)
		reset_base_color_component(data, key);
}

/* Handle reset and exit keys */
void	handle_reset_and_exit(int key, t_data *data)
{
	if (key == K_R)
		reset_view(data);
	else if (key == K_ESC || key == K_Q)
		exit_fractol(data);
}

/* Handle movement keys */
void	handle_movement(int key, t_data *data)
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

/* Move the view offset */
void	move_offset(t_data *data, double x_step, double y_step)
{
	data->offset_x += x_step;
	data->offset_y += y_step;
	data->redraw = 1;
}

/* Handle zoom keys */
void	handle_zoom(int key, t_data *data)
{
	if (key == K_NUM_PLUS)
	{
		data->zoom *= ZOOM_IN;
		data->redraw = 1;
	}
	else if (key == K_NUM_MINUS)
	{
		data->zoom *= ZOOM_OUT;
		data->redraw = 1;
	}
}

/* Handle iteration adjustment keys */
void	handle_iter_adjustment(int key, t_data *data)
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

/* Switch to the next color palette */
void	switch_palette_next(t_data *data)
{
	if (data->palette_type + 1 >= PALETTE_COUNT)
		data->palette_type = 0;
	else
		data->palette_type++;
	data->redraw = 1;
}

/* Switch to the previous color palette */
void	switch_palette_prev(t_data *data)
{
	if (data->palette_type == 0)
		data->palette_type = PALETTE_COUNT - 1;
	else
		data->palette_type--;
	data->redraw = 1;
}

/* Adjust imaginary part of c */
void	adjust_c_imag(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_imag += delta;
	else
		data->offset_y += delta;
	data->redraw = 1;
}

/* Adjust real part of c */
void	adjust_c_real(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_real += delta;
	else
		data->offset_x += delta;
	data->redraw = 1;
}

/* Adjust base color component */
void	adjust_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_7)
	{
		if (data->base_color.r - 15 < 0)
			data->base_color.r = 0;
		else
			data->base_color.r -= 15;
	}
	else if (key == K_NUM_9)
	{
		if (data->base_color.r + 15 > 255)
			data->base_color.r = 255;
		else
			data->base_color.r += 15;
	}
	else if (key == K_NUM_4)
	{
		if (data->base_color.g - 15 < 0)
			data->base_color.g = 0;
		else
			data->base_color.g -= 15;
	}
	else if (key == K_NUM_6)
	{
		if (data->base_color.g + 15 > 255)
			data->base_color.g = 255;
		else
			data->base_color.g += 15;
	}
	else if (key == K_NUM_1)
	{
		if (data->base_color.b - 15 < 0)
			data->base_color.b = 0;
		else
			data->base_color.b -= 15;
	}
	else if (key == K_NUM_3)
	{
		if (data->base_color.b + 15 > 255)
			data->base_color.b = 255;
		else
			data->base_color.b += 15;
	}
	data->redraw = 1;
}

/* Reset base color component */
void	reset_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_8)
		data->base_color.r = 255;
	else if (key == K_NUM_5)
		data->base_color.g = 255;
	else if (key == K_NUM_2)
		data->base_color.b = 255;
	data->redraw = 1;
}

/* Mouse press event */
int	mouse_press(int button, int x, int y, t_data *data)
{
	if (button == 1)
	{
		data->is_selecting = 1;
		data->select_start_x = x;
		data->select_start_y = y;
		data->select_end_x = x;
		data->select_end_y = y;
	}
	return (0);
}

/* Mouse move event */
int	mouse_move(int x, int y, t_data *data)
{
	if (data->is_selecting)
	{
		data->select_end_x = x;
		data->select_end_y = y;
		data->redraw = 1;
	}
	return (0);
}

/* Mouse release event */
int	mouse_release(int button, int x, int y, t_data *data)
{
	if (button == 1 && data->is_selecting)
	{
		data->is_selecting = 0;
		data->select_end_x = x;
		data->select_end_y = y;
		zoom_to_selection(data);
	}
	return (0);
}

/* Render the next frame */
int	render_next_frame(t_data *data)
{
	if (!data->redraw)
		return (0);
	if (data->fast_mode)
		draw_fractal_with_iter(data, DEFAULT_PREVIEW_ITER);
	else
		draw_fractal_with_iter(data, data->max_iter);
	data->fast_mode = 0;
	data->redraw = 0;
	return (0);
}

/* Draw the fractal with a given iteration count */
void	draw_fractal_with_iter(t_data *data, int iter_count)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	draw_fractal(data, iter_count);
	if (data->is_selecting)
		draw_selection_rectangle(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	draw_info_strings(data);
}

/* Draw the fractal */
void	draw_fractal(t_data *data, int iter_count)
{
	int		y;
	double	scale;
	double	real_min;
	double	imag_min;

	scale = 4.0 / (WIN_WIDTH * data->zoom);
	real_min = data->offset_x - (WIN_WIDTH / 2.0) * scale;
	imag_min = data->offset_y - (WIN_HEIGHT / 2.0) * scale;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		draw_fractal_line(data, y, real_min, imag_min, scale, iter_count);
		y++;
	}
}

/* Draw a line of the fractal */
void	draw_fractal_line(t_data *data, int y,
	double real_min, double imag_min, double scale, int iter_count)
{
	t_fractal_vars	vars;

	vars.x = 0;
	while (vars.x < WIN_WIDTH)
	{
		vars.c_real = real_min + vars.x * scale;
		vars.c_imag = imag_min + y * scale;
		if (data->fractal_type == JULIA)
		{
			vars.z_real = vars.c_real;
			vars.z_imag = vars.c_imag;
			vars.iter = compute_julia(data, vars.z_real, vars.z_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == MANDELBROT
			|| data->fractal_type == TRICORN)
		{
			vars.iter = compute_mandelbrot(data, vars.c_real, vars.c_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == BURNING_SHIP)
		{
			vars.iter = compute_burning_ship(vars.c_real, vars.c_imag,
					&vars.z_real, &vars.z_imag, iter_count);
		}
		else if (data->fractal_type == LYAPUNOV)
		{
			vars.iter = compute_lyapunov(vars.c_real, vars.c_imag, iter_count);
		}
		else if (data->fractal_type == NEWTON)
		{
			vars.iter = compute_newton(vars.c_real, vars.c_imag, iter_count);
		}
		put_pixel(data, vars.x, y, get_color(vars.iter,
				data, vars.z_real, vars.z_imag, data->max_iter));
		vars.x++;
	}
}

/* Draw information strings on the window */
void	draw_info_strings(t_data *data)
{
	const char	*palette_names[PALETTE_COUNT] = {
		"Derivative Bailout", "Stripes", "Smooth", "Classic",
		"Logarithmic", "HSV", "Gradient", "Black & White",
		"Escape Time", "Continuous Potential", "Fire",
		"Interior Distance", "LCH Coloring",
		"Exp Cyclic LCH No Shading", "Exp Cyclic LCH Shading"
	};
	mlx_string_put(data->mlx, data->win, 10, 10, 0xFFFFFF,
		"Fract'ol - 42 Project");
	mlx_string_put(data->mlx, data->win, 10, 30, 0xFFFFFF,
		"Press R to reset view");
	mlx_string_put(data->mlx, data->win, 10, 50, 0xFFFFFF,
		"Use arrow keys or WASD to move");
	mlx_string_put(data->mlx, data->win, 10, 70, 0xFFFFFF,
		"Use + and - to zoom");
	mlx_string_put(data->mlx, data->win, 10, 90, 0xFFFFFF,
		"Palette: ");
	mlx_string_put(data->mlx, data->win, 100, 90, 0xFFFFFF,
		(char *)palette_names[data->palette_type]);
	draw_zoom_level(data);
}

/* Draw the zoom level */
void	draw_zoom_level(t_data *data)
{
	char	*str;
	char	*temp;

	temp = ft_ftoa(data->zoom, 6);
	str = ft_strjoin("Zoom: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, 110, 0xFFFFFF, str);
	free(str);
}

/* Draw the selection rectangle */
void	draw_selection_rectangle(t_data *data)
{
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	draw_rectangle_edges(data, x_start, y_start, x_end, y_end);
}

/* Get sorted selection coordinates */
void	get_sorted_selection(t_data *data, int *x_start, int *x_end,
	int *y_start, int *y_end)
{
	*x_start = data->select_start_x;
	*y_start = data->select_start_y;
	*x_end = data->select_end_x;
	*y_end = data->select_end_y;
	if (*x_start > *x_end)
		swap_int(x_start, x_end);
	if (*y_start > *y_end)
		swap_int(y_start, y_end);
}

/* Draw the edges of the rectangle */
void	draw_rectangle_edges(t_data *data, int x_start, int y_start,
	int x_end, int y_end)
{
	int	x;
	int	y;

	x = x_start;
	while (x <= x_end)
	{
		put_pixel(data, x, y_start, 0xFFFFFF);
		put_pixel(data, x, y_end, 0xFFFFFF);
		x++;
	}
	y = y_start;
	while (y <= y_end)
	{
		put_pixel(data, x_start, y, 0xFFFFFF);
		put_pixel(data, x_end, y, 0xFFFFFF);
		y++;
	}
}

/* Put a pixel on the image */
void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

/* Swap two integers */
void	swap_int(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/* Zoom to the selected area */
void	zoom_to_selection(t_data *data)
{
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return ;
	calculate_zoom_and_offset(data, x_start, x_end, y_start, y_end);
	data->redraw = 1;
}

/* Calculate the new zoom and offset based on selection */
void	calculate_zoom_and_offset(t_data *data, int x_start, int x_end,
	int y_start, int y_end)
{
	double	scale;
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;

	scale = 4.0 / (WIN_WIDTH * data->zoom);
	x_min = data->offset_x + (x_start - WIN_WIDTH / 2.0) * scale;
	x_max = data->offset_x + (x_end - WIN_WIDTH / 2.0) * scale;
	y_min = data->offset_y + (y_start - WIN_HEIGHT / 2.0) * scale;
	y_max = data->offset_y + (y_end - WIN_HEIGHT / 2.0) * scale;
	update_zoom_and_offset(data, x_min, x_max, y_min, y_max);
}

/* Update zoom and offset */
void	update_zoom_and_offset(t_data *data, double x_min, double x_max,
	double y_min, double y_max)
{
	double	new_zoom;
	double	new_offset_x;
	double	new_offset_y;

	new_zoom = 4.0 / (x_max - x_min);
	new_offset_x = (x_min + x_max) / 2.0;
	new_offset_y = (y_min + y_max) / 2.0;
	data->zoom = new_zoom;
	data->offset_x = new_offset_x;
	data->offset_y = new_offset_y;
}

/* Compute Julia set */
int	compute_julia(t_data *data, double z_real, double z_imag,
	double *final_z_real, double *final_z_imag, int iter_count)
{
	int		iter;
	double	tmp;

	iter = 0;
	while ((z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		tmp = z_real * z_real - z_imag * z_imag + data->c_real;
		z_imag = 2.0 * z_real * z_imag + data->c_imag;
		z_real = tmp;
		iter++;
	}
	*final_z_real = z_real;
	*final_z_imag = z_imag;
	return (iter);
}

/* Compute Mandelbrot set */
int	compute_mandelbrot(t_data *data, double c_real, double c_imag,
	double *final_z_real, double *final_z_imag, int iter_count)
{
	int		iter;
	double	z_real;
	double	z_imag;
	double	tmp;

	z_real = 0;
	z_imag = 0;
	iter = 0;
	while ((z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		tmp = z_real * z_real - z_imag * z_imag + c_real;
		if (data->fractal_type == TRICORN)
			z_imag = -2.0 * z_real * z_imag + c_imag;
		else
			z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp;
		iter++;
	}
	*final_z_real = z_real;
	*final_z_imag = z_imag;
	return (iter);
}

/* Compute Burning Ship fractal */
int	compute_burning_ship(double c_real, double c_imag,
	double *final_z_real, double *final_z_imag, int iter_count)
{
	int		iter;
	double	z_real;
	double	z_imag;
	double	tmp;

	z_real = 0;
	z_imag = 0;
	iter = 0;
	while ((z_real * z_real + z_imag * z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		z_real = fabs(z_real);
		z_imag = fabs(z_imag);
		tmp = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp;
		iter++;
	}
	*final_z_real = z_real;
	*final_z_imag = z_imag;
	return (iter);
}

/* Compute Lyapunov fractal */
int	compute_lyapunov(double x, double y, int iter_count)
{
	double	r[2];
	double	value;
	int		iter;
	int		seq_len;
	int		i;

	r[0] = x;
	r[1] = y;
	value = 0.5;
	iter = 0;
	seq_len = ft_strlen(LYAPUNOV_SEQUENCE);
	while (iter < iter_count)
	{
		i = iter % seq_len;
		value = r[LYAPUNOV_SEQUENCE[i] - 'A'] * value * (1 - value);
		if (value == 0 || value == 1)
			break ;
		iter++;
	}
	return (iter);
}

/* Compute Newton fractal */
int	compute_newton(double z_real, double z_imag, int iter_count)
{
	int		iter;
	double	old_real;
	double	old_imag;
	double	denominator;
	double	tmp;

	iter = 0;
	while (iter < iter_count)
	{
		old_real = z_real;
		old_imag = z_imag;
		denominator = 3 * (old_real * old_real - old_imag * old_imag)
			* (old_real * old_real - old_imag * old_imag);
		if (denominator == 0)
			break ;
		tmp = (2 * old_real * old_real * old_real
			- 2 * old_real * old_imag * old_imag - old_real)
			/ denominator;
		z_imag = (2 * old_imag * old_imag * old_imag
			- 2 * old_real * old_real * old_imag + old_imag)
			/ denominator;
		z_real = tmp;
		if ((old_real - z_real) * (old_real - z_real)
			+ (old_imag - z_imag) * (old_imag - z_imag) < 1e-6)
			break ;
		iter++;
	}
	return (iter);
}

/* Get color based on iteration and palette */
int	get_color(int iter, t_data *data,
	double z_real, double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_STRIPES)
		return (get_color_stripes(iter, data));
	else if (data->palette_type == PALETTE_SMOOTH)
		return (get_color_smooth(iter, data));
	else if (data->palette_type == PALETTE_CLASSIC)
		return (get_color_classic(iter, data));
	else if (data->palette_type == PALETTE_DERIVATIVE_BAILOUT)
		return (get_color_derivative_bailout(iter, z_real, z_imag, data));
	else if (data->palette_type == PALETTE_LOGARITHMIC)
		return (get_color_logarithmic(iter, max_iter, data));
	else if (data->palette_type == PALETTE_HSV)
		return (get_color_hsv(iter, max_iter, data));
	else if (data->palette_type == PALETTE_GRADIENT)
		return (get_color_gradient(iter, max_iter, data));
	else if (data->palette_type == PALETTE_BLACK_WHITE)
		return (get_color_black_white(iter, data));
	else if (data->palette_type == PALETTE_ESCAPE_TIME)
		return (get_color_escape_time(iter, max_iter, data));
	else if (data->palette_type == PALETTE_CONTINUOUS_POTENTIAL)
		return (get_color_continuous_potential(iter, z_real,
				z_imag, max_iter, data));
	else if (data->palette_type == PALETTE_FIRE)
		return (get_color_fire(iter, max_iter, data));
	else if (data->palette_type == PALETTE_INTERIOR_DISTANCE)
		return (get_color_interior_distance(iter, z_real, z_imag,
				max_iter, data));
	else if (data->palette_type == PALETTE_LCH)
		return (get_color_lch(iter, max_iter, data));
	else if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_NO_SHADING)
		return (get_color_exp_cyclic_lch_no_shading(iter, max_iter, data));
	else if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_SHADING)
		return (get_color_exp_cyclic_lch_shading(iter, max_iter, data));
	return (0);
}

/* Get stripes color */
int	get_color_stripes(int iter, t_data *data)
{
	double	t;
	double	value;
	int		color;

	t = (double)iter / data->max_iter;
	value = sin(10.0 * t);
	color = (int)((1 + value) * 127.5);
	return ((color << 16) | (color << 8) | color);
}

/* Get smooth color */
int	get_color_smooth(int iter, t_data *data)
{
	double	t;
	int		r;
	int		g;
	int		b;

	t = (double)iter / data->max_iter;
	r = (int)(9 * (1 - t) * t * t * t * 255);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((r << 16) | (g << 8) | b);
}

/* Get classic color */
int	get_color_classic(int iter, t_data *data)
{
	int	color;

	color = (iter * 255 / data->max_iter) % 256;
	return ((color << 16) | (color << 8) | color);
}

/* Get logarithmic color */
int	get_color_logarithmic(int iter, int max_iter, t_data *data)
{
	double	normalized;
	int		color;

	if (iter < max_iter)
		normalized = log((double)iter) / log((double)max_iter);
	else
		normalized = 0;
	color = (int)(normalized * 255);
	return ((color << 16) | (color << 8) | color);
}

/* Get HSV color */
int	get_color_hsv(int iter, int max_iter, t_data *data)
{
	double	t;
	double	h;
	double	s;
	double	v;
	int		r;
	int		g;
	int		b;

	t = (double)iter / max_iter;
	h = t * 360;
	s = 1.0;
	v = 1.0;
	hsv_to_rgb(h, s, v, &r, &g, &b);
	r = (r * data->base_color.r) / 255;
	g = (g * data->base_color.g) / 255;
	b = (b * data->base_color.b) / 255;
	return ((r << 16) | (g << 8) | b);
}

/* Convert HSV to RGB */
void	hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b)
{
	double	c;
	double	x;
	double	m;
	double	rgb_prime[3];
	int		sector;

	c = v * s;
	x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
	m = v - c;
	rgb_prime[0] = 0;
	rgb_prime[1] = 0;
	rgb_prime[2] = 0;
	sector = (int)(h / 60.0) % 6;
	if (sector == 0)
	{
		rgb_prime[0] = c;
		rgb_prime[1] = x;
		rgb_prime[2] = 0;
	}
	else if (sector == 1)
	{
		rgb_prime[0] = x;
		rgb_prime[1] = c;
		rgb_prime[2] = 0;
	}
	else if (sector == 2)
	{
		rgb_prime[0] = 0;
		rgb_prime[1] = c;
		rgb_prime[2] = x;
	}
	else if (sector == 3)
	{
		rgb_prime[0] = 0;
		rgb_prime[1] = x;
		rgb_prime[2] = c;
	}
	else if (sector == 4)
	{
		rgb_prime[0] = x;
		rgb_prime[1] = 0;
		rgb_prime[2] = c;
	}
	else if (sector == 5)
	{
		rgb_prime[0] = c;
		rgb_prime[1] = 0;
		rgb_prime[2] = x;
	}
	*r = (int)((rgb_prime[0] + m) * 255);
	*g = (int)((rgb_prime[1] + m) * 255);
	*b = (int)((rgb_prime[2] + m) * 255);
}

/* Get gradient color */
int	get_color_gradient(int iter, int max_iter, t_data *data)
{
	double	t;
	int		r_start;
	int		g_start;
	int		b_start;
	int		r_end;
	int		g_end;
	int		b_end;
	int		r;
	int		g;
	int		b;

	t = (double)iter / max_iter;
	r_start = 0;
	g_start = 0;
	b_start = 255;
	r_end = 255;
	g_end = 165;
	b_end = 0;
	r = (int)((1 - t) * r_start + t * r_end);
	g = (int)((1 - t) * g_start + t * g_end);
	b = (int)((1 - t) * b_start + t * b_end);
	r = (r * data->base_color.r) / 255;
	g = (g * data->base_color.g) / 255;
	b = (b * data->base_color.b) / 255;
	return ((r << 16) | (g << 8) | b);
}

/* Get black and white color */
int	get_color_black_white(int iter, t_data *data)
{
	if (iter % 2 == 0)
		return ((data->base_color.r << 16)
				| (data->base_color.g << 8) | data->base_color.b);
	else
		return (0x000000);
}

/* Get escape time color */
int	get_color_escape_time(int iter, int max_iter, t_data *data)
{
	double	t;
	int		color;

	t = (double)iter / max_iter;
	color = (int)(t * 255);
	color = (color * data->base_color.r) / 255;
	return ((color << 16) | (color << 8) | color);
}

/* Get continuous potential color */
int	get_color_continuous_potential(int iter, double z_real,
	double z_imag, int max_iter, t_data *data)
{
	double	zn;
	double	nu;
	double	t;
	int		r;
	int		g;
	int		b;

	zn = sqrt(z_real * z_real + z_imag * z_imag);
	nu = log(log(zn) / log(2)) / log(2);
	t = iter + 1 - nu;
	t = t / max_iter;
	r = (int)(255 * t * (data->base_color.r / 255.0));
	g = (int)(255 * (1 - t) * (data->base_color.g / 255.0));
	b = (int)(255 * (t * (1 - t)) * (data->base_color.b / 255.0));
	return ((r << 16) | (g << 8) | b);
}

/* Get fire color */
int	get_color_fire(int iter, int max_iter, t_data *data)
{
	double	t;
	int		r;
	int		g;
	int		b;

	t = (double)iter / max_iter;
	r = (int)(255 * pow(t, 0.3));
	g = (int)(255 * pow(t, 1.0));
	b = (int)(255 * pow(t, 3.0));
	return ((r << 16) | (g << 8) | b);
}

/* Get derivative bailout color */
int	get_color_derivative_bailout(int iter,
	double z_real, double z_imag, t_data *data)
{
	double	magnitude;
	int		color;

	magnitude = sqrt(z_real * z_real + z_imag * z_imag);
	color = (int)(255 * (1 - exp(-magnitude)));
	return ((color << 16) | (color << 8) | color);
}

/* Get interior distance color */
int	get_color_interior_distance(int iter, double z_real, double z_imag,
	int max_iter, t_data *data)
{
	double	distance;
	double	normalized;
	int		color_value;
	int		r;
	int		g;
	int		b;

	if (iter == max_iter)
	{
		distance = sqrt(z_real * z_real + z_imag * z_imag);
		normalized = log(distance) / log(ESCAPE_RADIUS);
		color_value = (int)(normalized * 255) % 256;
		r = color_value;
		g = color_value;
		b = color_value;
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}
	return ((r << 16) | (g << 8) | b);
}

/* Get LCH color */
int	get_color_lch(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;

	vars.t = (double)iter / max_iter;
	vars.l = 50 + 50 * sin(3.1415 * vars.t);
	vars.c = 50;
	vars.h = 360 * vars.t;
	lch_to_rgb(vars.l, vars.c, vars.h, &vars.r, &vars.g, &vars.b);
	vars.r = (vars.r * data->base_color.r) / 255;
	vars.g = (vars.g * data->base_color.g) / 255;
	vars.b = (vars.b * data->base_color.b) / 255;
	return ((vars.r << 16) | (vars.g << 8) | vars.b);
}

/* Get exponential cyclic LCH color without shading */
int	get_color_exp_cyclic_lch_no_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;

	vars.t = exp((double)-iter / max_iter);
	vars.l = 70;
	vars.c = 50;
	vars.h = fmod(360 * vars.t * 10, 360);
	lch_to_rgb(vars.l, vars.c, vars.h, &vars.r, &vars.g, &vars.b);
	vars.r = (vars.r * data->base_color.r) / 255;
	vars.g = (vars.g * data->base_color.g) / 255;
	vars.b = (vars.b * data->base_color.b) / 255;
	return ((vars.r << 16) | (vars.g << 8) | vars.b);
}

/* Get exponential cyclic LCH color with shading */
int	get_color_exp_cyclic_lch_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;

	vars.t = exp((double)-iter / max_iter);
	vars.l = 50 + 20 * sin(2 * 3.1415 * vars.t);
	vars.c = 30 + 20 * sin(2 * 3.1415 * vars.t);
	vars.h = fmod(360 * vars.t * 10, 360);
	lch_to_rgb(vars.l, vars.c, vars.h, &vars.r, &vars.g, &vars.b);
	vars.r = (vars.r * data->base_color.r) / 255;
	vars.g = (vars.g * data->base_color.g) / 255;
	vars.b = (vars.b * data->base_color.b) / 255;
	return ((vars.r << 16) | (vars.g << 8) | vars.b);
}

/* Convert LCH to RGB */
void	lch_to_rgb(double l, double c, double h_deg, int *r, int *g, int *b)
{
	double	a;
	double	b_lab;
	double	x;
	double	y;
	double	z;

	lch_to_lab(l, c, h_deg, &a, &b_lab);
	lab_to_xyz(l, a, b_lab, &x, &y, &z);
	xyz_to_rgb(x, y, z, r, g, b);
}

/* Convert LCH to LAB */
void	lch_to_lab(double l, double c, double h_deg,
	double *a, double *b_lab)
{
	double	h_rad;

	h_rad = h_deg * (M_PI / 180.0);
	*a = cos(h_rad) * c;
	*b_lab = sin(h_rad) * c;
}

/* Convert LAB to XYZ */
void	lab_to_xyz(double l, double a, double b_lab,
	double *x, double *y, double *z)
{
	double	fy;
	double	fx;
	double	fz;

	fy = (l + 16.0) / 116.0;
	fx = fy + (a / 500.0);
	fz = fy - (b_lab / 200.0);
	if (pow(fx, 3) > 0.008856)
		*x = pow(fx, 3);
	else
		*x = (fx - 16.0 / 116.0) / 7.787;
	if (pow(fy, 3) > 0.008856)
		*y = pow(fy, 3);
	else
		*y = (fy - 16.0 / 116.0) / 7.787;
	if (pow(fz, 3) > 0.008856)
		*z = pow(fz, 3);
	else
		*z = (fz - 16.0 / 116.0) / 7.787;
	*x *= 95.047;
	*y *= 100.0;
	*z *= 108.883;
}

/* Convert XYZ to RGB */
void	xyz_to_rgb(double x, double y, double z, int *r, int *g, int *b)
{
	double	r_lin;
	double	g_lin;
	double	b_lin;

	x /= 100.0;
	y /= 100.0;
	z /= 100.0;
	r_lin = x *  3.2406 + y * (-1.5372) + z * (-0.4986);
	g_lin = x * (-0.9689) + y *  1.8758 + z *  0.0415;
	b_lin = x *  0.0557 + y * (-0.2040) + z *  1.0570;
	if (r_lin > 0.0031308)
		r_lin = 1.055 * pow(r_lin, 1 / 2.4) - 0.055;
	else
		r_lin = 12.92 * r_lin;
	if (g_lin > 0.0031308)
		g_lin = 1.055 * pow(g_lin, 1 / 2.4) - 0.055;
	else
		g_lin = 12.92 * g_lin;
	if (b_lin > 0.0031308)
		b_lin = 1.055 * pow(b_lin, 1 / 2.4) - 0.055;
	else
		b_lin = 12.92 * b_lin;
	*r = (int)(fmin(fmax(r_lin, 0.0), 1.0) * 255);
	*g = (int)(fmin(fmax(g_lin, 0.0), 1.0) * 255);
	*b = (int)(fmin(fmax(b_lin, 0.0), 1.0) * 255);
}

/* Convert double to string with specified precision */
char	*ft_ftoa(double n, int precision)
{
	long long	int_part;
	double		frac_part;
	char		*int_str;
	char		*frac_str;
	char		*result;
	int			i;

	int_part = (long long)n;
	frac_part = n - (double)int_part;
	if (frac_part < 0)
		frac_part = -frac_part;
	int_str = ft_lltoa(int_part);
	if (!int_str)
		return (NULL);
	frac_str = (char *)malloc(precision + 2);
	if (!frac_str)
	{
		free(int_str);
		return (NULL);
	}
	frac_str[0] = '.';
	i = 1;
	while (i <= precision)
	{
		frac_part *= 10;
		frac_str[i] = ((int)frac_part % 10) + '0';
		frac_part -= (int)frac_part;
		i++;
	}
	frac_str[i] = '\0';
	result = ft_strjoin(int_str, frac_str);
	free(int_str);
	free(frac_str);
	return (result);
}

/* Convert long long to string */
char	*ft_lltoa(long long n)
{
	char		*str;
	int			len;
	int			sign;
	long long	nb;

	sign = 0;
	if (n < 0)
	{
		sign = 1;
		nb = -n;
	}
	else
		nb = n;
	len = ft_numlen(n);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len-- > sign)
	{
		str[len] = '0' + (nb % 10);
		nb /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

/* Get the length of a number */
static int	ft_numlen(long long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/* Join two strings and free as needed */
char	*ft_strjoin_free(char *s1, char *s2, int free_s1)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (free_s1)
		free(s1);
	free(s2);
	return (result);
}

