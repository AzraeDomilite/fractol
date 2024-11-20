/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:34:36 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:34:36 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../mlx/mlx.h"
# include "../libs/libft/src/libft.h"
# include "../libs/ft_printf/include/ft_printf.h"
# include <unistd.h>
# include <math.h>
# include <stdlib.h>
# include <pthread.h>
#include <complex.h>

/* ************************************************************************** */
/*                                   WINDOW                                   */
/* ************************************************************************** */

# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define FULLSCREEN_WIDTH	1920
# define FULLSCREEN_HEIGHT	1080

/* ************************************************************************** */
/*                                ERROR MESSAGES                              */
/* ************************************************************************** */

# define USAGE_MSG "Usage: ./fractol <julia|mandelbrot|tricorn|burningship|lyapunov|newton|buddhabrot>\n"
# define ERROR_MSG_WINDOW "Error\nWindow creation failed\n"
# define ERROR_MSG_IMAGE "Error\nImage creation failed\n"
# define ERROR_MSG_LOOP "Error\nProgram failed on loop\n"
# define ERROR_MLX_INIT "Error\nFailed to initialize MLX\n"
# define ERROR_MUTEX_INIT "Error\nMutex initialization failed\n"
# define ERROR_THREAD_CREATE "Error\nThread creation failed\n"
# define ERROR_MALLOC_HIST "Error\nMemory allocation failed for histogram\n"
# define ERROR_MALLOC_THREAD "Error\nThread allocation failed\n"
# define ERROR_MALLOC_THREAD_DATA "Error\nThread data allocation failed\n"

/* ************************************************************************** */
/*                            FRACTAL PARAMETERS                              */
/* ************************************************************************** */

/* Iteration Settings */
# define MAX_ITER 10000
# define MIN_ITER 10
# define ITER_STEP 50
# define DEFAULT_PREVIEW_ITER 100

/* View & Navigation */
# define ZOOM_FACTOR 1.2
# define MOVE_FACTOR 0.10
# define ESCAPE_RADIUS 4.0

/* Color Adjustment */
# define COLOR_ADJUST_STEP 15

/* Julia Set Default Values */
# define INITIAL_C_REAL -0.7
# define INITIAL_C_IMAG 0.27015

/* View Boundaries */
# define VIEW_X_MIN -2.5
# define VIEW_X_MAX 1.0
# define VIEW_Y_MIN -1.5
# define VIEW_Y_MAX 1.5

/* Mandelbrot View Offset */
# define MANDEL_OFFSET_X -0.5
# define MANDEL_OFFSET_Y 0.0

/* ************************************************************************** */
/*                             THREADING PARAMETERS                           */
/* ************************************************************************** */

# define NUM_THREADS 8
# define SAMPLES_PER_THREAD 500000
# define THREAD_STACK_SIZE 16777216

/* ************************************************************************** */
/*                                BUDDHABROT                                  */
/* ************************************************************************** */

# define BUDDHA_REAL_MIN -2.5
# define BUDDHA_REAL_MAX 1.0
# define BUDDHA_IMAG_MIN -1.5
# define BUDDHA_IMAG_MAX 1.5
# define BUDDHA_SAMPLE_RANGE_X 3.0
# define BUDDHA_SAMPLE_RANGE_Y 3.0
# define BUDDHA_OFFSET_X -2.0
# define BUDDHA_OFFSET_Y -1.5

/* ************************************************************************** */
/*                                  LYAPUNOV                                  */
/* ************************************************************************** */

# define LYAPUNOV_SEQUENCE "AAAA"

/* ************************************************************************** */
/*                                   COLORS                                   */
/* ************************************************************************** */

# define PALETTE_COUNT 17
# define MAX_COLOR_VALUE 255
# define COLOR_WHITE 0xFFFFFF
# define COLOR_BLACK 0x000000

/* HSV Color Space */
# define HSV_HUE_MAX 360.0
# define HSV_SATURATION 1.0
# define HSV_VALUE 1.0

/* LCH Color Space */
# define LCH_L_BASE 50.0
# define LCH_L_RANGE 50.0
# define LCH_C_BASE 50.0
# define LCH_H_MULTIPLIER 10.0

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

# define ADJUST_COLOR_COMPONENT(color, delta) \
	do { \
		if ((color) + (delta) > MAX_COLOR_VALUE) \
			(color) = MAX_COLOR_VALUE; \
		else if ((color) + (delta) < 0) \
			(color) = 0; \
		else \
			(color) += (delta); \
	} while (0)

# define MIN(a,b) ((a) < (b) ? (a) : (b))
# define MAX(a,b) ((a) > (b) ? (a) : (b))

/* ************************************************************************** */
/*                                   KEYS                                     */
/* ************************************************************************** */

# define K_ESC			53
# define K_R			15
# define K_Q			12
# define K_W			13
# define K_A			0
# define K_S			1
# define K_D			2
# define K_O			31
# define K_I			34
# define K_K			40
# define K_L			37
# define K_F			3
# define K_G			5
# define K_Z			6
# define K_X			7
# define K_C			8
# define K_V			9
# define K_H			4
# define K_1			18
# define K_2			19
# define K_3			20
# define K_4			21
# define K_5			23
# define K_6			22
# define K_7			26
# define K_8			28
# define K_9			25
# define K_0			29
# define K_NUM_1		83
# define K_NUM_2		84
# define K_NUM_3		85
# define K_NUM_4		86
# define K_NUM_5		87
# define K_NUM_6		88
# define K_NUM_7		89
# define K_NUM_8		91
# define K_NUM_9		92
# define K_NUM_0		82
# define K_POINT		47
# define K_COMMA		43
# define K_NUM_MULT		67
# define K_NUM_DIV		75
# define K_NUM_PLUS		69
# define K_NUM_MINUS	78
# define K_NUM_ENTER	76
# define K_NUM_0		82
# define K_UP			126
# define K_DOWN			125
# define K_LEFT			123
# define K_RIGHT		124
# define K_DIGIT_1		18

/* ************************************************************************** */
/*                                   TYPES                                    */
/* ************************************************************************** */
struct	s_data;
typedef struct	s_data t_data;

typedef struct s_color_char
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_color_char;

typedef enum e_fractal_type
{
	JULIA,
	MANDELBROT,
	TRICORN,
	BURNING_SHIP,
	LYAPUNOV,
	NEWTON,
	BUDDHABROT
}	t_fractal_type;

typedef struct s_trajectory
{
	double	real[MAX_ITER];
	double	imag[MAX_ITER];
}	t_trajectory;

typedef struct s_thread_data
{
	t_data			*data;
	unsigned int	seed;
	int				samples;
	double			*traj_real;
	double			*traj_imag;
	unsigned int	*local_histogram;
}	t_thread_data;


typedef enum e_palette_type
{
	PALETTE_FIRE,
	PALETTE_STRIPES,
	PALETTE_SMOOTH,
	PALETTE_CLASSIC,
	PALETTE_LOGARITHMIC,
	PALETTE_HSV,
	PALETTE_GRADIENT,
	PALETTE_BLACK_WHITE,
	PALETTE_ESCAPE_TIME,
	PALETTE_CONTINUOUS_POTENTIAL,
	PALETTE_INTERIOR_DISTANCE,
	PALETTE_LCH,
	PALETTE_EXP_CYCLIC_LCH_NO_SHADING,
	PALETTE_EXP_CYCLIC_LCH_SHADING,
	PALETTE_DERIVATIVE_BAILOUT,
	PALETTE_DWELL_GRADIENT,
	PALETTE_CUSTOM_INTERIOR
}	t_palette_type;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_data
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				width;
	int				height;
	int				original_width;
	int				original_height;
	int				is_fullscreen;

	double			zoom;
	double			offset_x;
	double			offset_y;
	double			c_real;
	double			c_imag;
	double			real_min;
	double			imag_min;
	double			scale;
	int				max_iter;
	int				iter_count;
	t_fractal_type	fractal_type;

	t_palette_type	palette_type;
	t_color			base_color;
	double			branch_factor;
	double			branch_offset;
	double			branch_base;
	double			line_width_base;
	double			line_brightness;
	double			cell_brightness;
	double			base_saturation;

	double			buddha_real_min;
	double			buddha_real_max;
	double			buddha_imag_min;
	double			buddha_imag_max;
	pthread_mutex_t	histogram_mutex;
	unsigned int	*histogram;
	int				samples;

	int				is_selecting;
	int				select_start_x;
	int				select_start_y;
	int				select_end_x;
	int				select_end_y;

	int				redraw;
	int				fast_mode;
	int				overlay_enabled;
	int				y_start;
	int				y_end;
}	t_data;

typedef struct s_fractal_vars
{
	int		x;
	int		iter;
	double	c_real;
	double	c_imag;
	double	z_real;
	double	z_imag;
}	t_fractal_vars;

typedef struct s_fractal_params
{
	double	real_min;
	double	imag_min;
	double	scale;
	int		iter_count;
}	t_fractal_params;

typedef struct s_color_vars
{
	double	t;
	double	h;
}	t_color_vars;

typedef struct s_lch_color
{
	double	l;
	double	c;
	double	h_deg;
}	t_lch_color;

/* ************************************************************************** */
/*                               FUNCTION PROTOTYPES                          */
/* ************************************************************************** */

/* main.c */
int		main(int argc, char **argv);

/* init.c */
void	init_data(t_data *data);
static void	init_data_colors(t_data *data);
static void	init_data_dimensions(t_data *data);
static void	init_data_params(t_data *data);
void	parse_arguments(int argc, char **argv, t_data *data);

/* mlx_setup.c */
void	initialize_mlx(t_data *data);
void	reset_view(t_data *data);
void	exit_fractol(t_data *data);

/* event_handlers.c */
int	deal_key(int key, t_data *data);
int	mouse_press(int button, int x, int y, t_data *data);
int	mouse_move(int x, int y, t_data *data);
int	mouse_release(int button, int x, int y, t_data *data);

/* key_handlers_1.c */
void	adjust_fractal_parameters(int key, t_data *data);
void	handle_reset_and_exit(int key, t_data *data);
void	handle_movement(int key, t_data *data);
void	handle_zoom(int key, t_data *data);
void	handle_iter_adjustment(int key, t_data *data);

/* key_handlers_2.c */
static void	handle_fast_mode(t_data *data);
static void	handle_overlay(t_data *data);
static void	handle_screen_mode(t_data *data);
void	enter_fullscreen(t_data *data);
void	exit_fullscreen(t_data *data);

/* color_handlers.c */
void	switch_palette_next(t_data *data);
void	switch_palette_prev(t_data *data);
void	adjust_c_imag(t_data *data, double delta);
void	adjust_c_real(t_data *data, double delta);
void	adjust_base_color_component(t_data *data, int key);

/* color_handlers_2.c */
void	reset_base_color_component(t_data *data, int key);

/* drawing.c */
int		render_next_frame(t_data *data);
void	draw_fractal_with_iter(t_data *data, int iter_count);
void	draw_fractal(t_data *data, int iter_count);
void	*thread_draw_fractal(void *arg);
void	draw_fractal_line(t_data *data, int y, t_fractal_params *params);

/* color_utils_1.c */
int	get_color(int iter, t_data *data, double z_real, double z_imag, int max_iter);
static int	get_color_part1(int iter, t_data *data, double z_real, double z_imag, int max_iter);
static int	get_color_part2(int iter, t_data *data, double z_real, double z_imag, int max_iter);
static int	get_color_part3(int iter, t_data *data, double z_real, double z_imag, int max_iter);
void	put_pixel(t_data *data, int x, int y, int color);

/* color_utils_2.c */
void	hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b);
void	lch_to_rgb(t_lch_color *lch, int *r, int *g, int *b);
void	yuv_to_rgb(double y, double u, double v, int *r, int *g, int *b);
void	hsv2rgb(float h, float s, float v, float *red, float *grn, float *blu);

/* color_palettes_1.c */
int	get_color_fire(int iter, int max_iter, t_data *data);
int	get_color_stripes(int iter, t_data *data);
int	get_color_smooth(int iter, t_data *data);
int	get_color_classic(int iter, t_data *data);
int	get_color_logarithmic(int iter, int max_iter, t_data *data);

/* color_palettes_2.c */
int	get_color_hsv(int iter, int max_iter, t_data *data);
int	get_color_gradient(int iter, int max_iter, t_data *data);
int	get_color_black_white(int iter, t_data *data);
int	get_color_escape_time(int iter, int max_iter, t_data *data);
int	get_color_continuous_potential(int iter, double z_real, double z_imag, int max_iter, t_data *data);

/* color_palettes_3.c */
int	get_color_interior_distance(int iter, double z_real, double z_imag, int max_iter, t_data *data);
int	get_color_lch(int iter, int max_iter, t_data *data);
int	get_color_exp_cyclic_lch_no_shading(int iter, int max_iter, t_data *data);
int	get_color_exp_cyclic_lch_shading(int iter, int max_iter, t_data *data);
int	get_color_derivative_bailout(int iter, double z_real, double z_imag, t_data *data);

/* color_palettes_4.c */
int	get_color_custom_interior(int iter, int max_iter, t_data *data, double z_real, double z_imag);
void	dwell_gradient(int width, int height, int maxiter, int i, int j, const int *counts, unsigned char *pixel, t_data *data);
int	get_color_dwell_gradient(int iter, int max_iter, t_data *data, double z_real, double z_imag);
double	m_continuous_dwell(int N, double R, double _Complex c);
double	_Complex	m_dwell_gradient(int N, double R, double s, double d, double _Complex c);

/* ui_drawing_1.c */
void	draw_info_strings(t_data *data);
void	draw_controls(t_data *data, int *y);
void	draw_parameters(t_data *data, int *y);
static void	draw_zoom_info(t_data *data, int *y);
static void	draw_iter_info(t_data *data, int *y);

/* ui_drawing_2.c */
static void	draw_rgb_info(t_data *data, int *y);
static char	*create_rgb_string(t_data *data);
void	draw_fractal_type(t_data *data, int *y);
void	draw_palette_type(t_data *data, int *y);
void	draw_selection_rectangle(t_data *data);

/* selection.c */
void	get_sorted_selection(t_data *data, int *x_start, int *x_end, int *y_start, int *y_end);
void	draw_rectangle_edges(t_data *data, int x_start, int y_start, int x_end, int y_end);
void	swap_int(int *a, int *b);
void	zoom_to_selection(t_data *data);
void	calculate_zoom_and_offset(t_data *data, int x_start, int x_end, int y_start, int y_end);

/* compute.c */
int	compute_fractal(t_data *data, t_fractal_vars *vars, int iter_count);

/* julia.c */
int	compute_julia(t_data *data, t_fractal_vars *vars, int iter_count);

/* mandelbrot_1.c */
int	compute_mandelbrot(t_data *data, t_fractal_vars *vars, int iter_count);
static void	init_mandel_vars(t_fractal_vars *vars);
static int	compute_tricorn(t_fractal_vars *vars, int iter_count);
static int	compute_standard(t_fractal_vars *vars, int iter_count);
int	compute_burning_ship(t_fractal_vars *vars, int iter_count);

/* lyapunov.c */
int	compute_lyapunov(t_fractal_vars *vars, int iter_count);
static void	init_lyap_vars(double *value, double *sum_log_deriv, int *max_iter, int iter_count);
static int	check_value_bounds(double value, double r);
static int	calculate_final_value(double sum_log_deriv, int iter);
int	compute_newton(t_fractal_vars *vars, int iter_count);

/* newton.c */
static void	init_newton_vars(t_fractal_vars *vars);
static double	calculate_denominator(double old_real, double old_imag);
static void	calculate_next_z(t_fractal_vars *vars, double old_real, double old_imag, double denominator);

/* buddhabrot_1.c */
void	render_buddhabrot(t_data *data);
static int	init_main_histogram(t_data *data);
static int	init_thread_resources(t_thread_data **thread_data, pthread_t **threads);
static int	init_thread_data_arrays(t_thread_data *thread_data, t_data *data, int i);
static int	create_and_run_threads(t_thread_data *thread_data, pthread_t *threads, t_data *data);

/* buddhabrot_2.c */
void	render_buddhabrot_image(t_data *data);
unsigned int	find_max_value(unsigned int *array, int size);
void	cleanup_buddhabrot(t_data *data, pthread_t *threads, t_thread_data *thread_data);
void	*thread_generate_buddhabrot(void *arg);
void	*process_buddhabrot_section(void *arg);

/* buddhabrot_3.c */
void	merge_local_histograms(t_data *data, t_thread_data *thread_data, int num_threads);
void	process_point(t_data *data, double c_real, double c_imag);
void	update_histogram(t_data *data, double *traj_real, double *traj_imag, int length);
void	normalize_and_render_buddhabrot(t_data *data);
void	process_buddhabrot_point(t_data *data, double c_real, double c_imag);

/* utils_1.c */
void	ft_swap(int *a, int *b);
int	ft_clamp(int value, int min, int max);
char	*ft_ftoa(double n, int precision);
char	*ft_lltoa(long long n);
int	ft_numlen(long long n);

/* utils_2.c */
char	*str_join_free(char *s1, char *s2);
static int	check_escape(double z_real, double z_imag);
static void	calculate_next_point(double *z_real, double *z_imag, double c_real, double c_imag);
int	is_in_main_cardioid(double x, double y);
int	is_in_period2_bulb(double x, double y);

/* utils_3.c */
static void	update_histogram_point(t_data *data, int screen_x, int screen_y);
static void	calculate_screen_coords(double real, double imag, int *screen_x, int *screen_y);
void	move_offset(t_data *data, double x_factor, double y_factor);
void	handle_zoom(int key, t_data *data);
static void	calculate_buddha_bounds(t_data *data, int x_start, int x_end, int y_start, int y_end);

/* utils_4.c */
static void	calculate_coordinates(t_data *data, int x, int y, double *coord_x, double *coord_y);
static void	update_zoom_and_center(t_data *data, double start_x, double end_x, double start_y, double end_y);
void update_zoom_and_offset(t_data *data, double x_min, double x_max, double y_min, double y_max);
#endif