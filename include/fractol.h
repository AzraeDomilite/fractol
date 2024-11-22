/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:38:11 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:38:11 by blucken          ###   ########.ch       */
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
# include <complex.h>

/* ************************************************************************** */
/*                                   WINDOW                                   */
/* ************************************************************************** */

# define WIN_WIDTH 1300
# define WIN_HEIGHT 800
# define FULLSCREEN_WIDTH	3840
# define FULLSCREEN_HEIGHT	2160

/* ************************************************************************** */
/*                                ERROR MESSAGES                              */
/* ************************************************************************** */

# define USAGE_MSG "Usage: ./fractol <julia|mandelbrot|tricorn|\
burningship|lyapunov|newton|buddhabrot>\n"
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
# define MAX_ITER 2000
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
/*                               GRADIENT COLORS                              */
/* ************************************************************************** */

/* Exterior RGB Values */
# define GRAD_R_EXT 0
# define GRAD_G_EXT 0
# define GRAD_B_EXT 0

/* Interior RGB Values */
# define GRAD_R_INT 125
# define GRAD_G_INT 249
# define GRAD_B_INT 255

/* ************************************************************************** */
/*                           DWELL GRADIENT PARAMS                            */
/* ************************************************************************** */

# define DWELL_R 4.0
# define DWELL_S 1.0
# define DWELL_D 0.1

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

typedef struct s_data	t_data;

typedef struct s_color_char
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_color_char;

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
}			t_trajectory;

typedef struct s_thread_data
{
	t_data			*data;
	unsigned int	seed;
	int				samples;
	double			*traj_real;
	double			*traj_imag;
	unsigned int	*local_histogram;
}					t_thread_data;

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
}		t_color;

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
}					t_data;

typedef struct s_buddhabrot_point
{
	double	z_real;
	double	z_imag;
	double	tmp;
	double	trajectory_real[MAX_ITER];
	double	trajectory_imag[MAX_ITER];
	int		iter;
}			t_buddhabrot_point;

typedef struct s_fractal_vars
{
	int		x;
	int		iter;
	double	c_real;
	double	c_imag;
	double	z_real;
	double	z_imag;
}			t_fractal_vars;

typedef struct s_fractal_params
{
	double	real_min;
	double	imag_min;
	double	scale;
	int		iter_count;
}			t_fractal_params;

typedef struct s_color_vars
{
	double	t;
	double	h;
}			t_color_vars;

typedef struct s_lch_color
{
	double	l;
	double	c;
	double	h_deg;
}			t_lch_color;

typedef struct s_mouse_data
{
	int		x;
	int		y;
	double	mouse_re;
	double	mouse_im;
	double	zoom_factor;
}			t_mouse_data;

typedef struct s_lyap_vars
{
	double			value;
	double			sum_log_deriv;
	int				iter;
	int				max_iter;
	double			r;
	unsigned int	m;
	char			*sequence;
}					t_lyap_vars;

typedef struct s_hsv
{
	double	c;
	double	x;
	double	m;
	int		hi;
}			t_hsv;

typedef struct s_hsv_to_rgb
{
	float	p;
	float	q;
	float	t;
	float	r;
	float	g;
	float	b;
	int		case_value;
}			t_hsv_to_rgb;

typedef struct s_zoom
{
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;
	double	start_x;
	double	start_y;
	double	end_x;
	double	end_y;
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
}			t_zoom;

typedef struct s_zoom_calc
{
	double	selected_width;
	double	selected_height;
	double	zoom_factor_x;
	double	zoom_factor_y;
	double	new_center_x;
	double	new_center_y;
}			t_zoom_calc;

typedef struct s_bounds
{
	double	x_range;
	double	y_range;
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
}			t_bounds;

typedef struct s_color_interior
{
	double	magnitude_sq;
	double	magnitude_val;
	double	escape_value;
	double	angle;
	double	u;
	double	v;
	int		r;
	int		g;
	int		b;
}	t_color_interior;

typedef struct s_draw_rect
{
	int	x;
	int	y;
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;
}		t_draw_rect;

typedef struct s_color_args
{
	int		iter;
	t_data	*data;
	double	z_real;
	double	z_imag;
	int		max_iter;
}			t_color_args;

typedef struct s_color_potential
{
	double	zn;
	double	nu;
	double	t;
	int		r;
	int		g;
	int		b;
}			t_color_potential;

typedef struct s_draw_line
{
	t_fractal_vars	vars;
	t_color_args	args;
	int				x;
}					t_draw_line;

typedef struct s_color_f
{
	float	r;
	float	g;
	float	b;
}			t_color_f;

typedef struct s_gradient_data
{
	double _Complex		c;
	double _Complex		gradient;
	double _Complex		z;
	double				vz;
	double				vx;
	double				vy;
	double				vm;
	double				m;
	double				mx;
	double				my;
	double				hue;
	double				saturation;
	double				value;
	float				r;
	float				g;
	float				b;
	t_color_f			color;

}					t_gradient_data;

typedef struct s_color_params
{
	int		iter;
	int		max_iter;
	double	z_real;
	double	z_imag;
}	t_color_params;

typedef struct s_gradient_params
{
	int		width;
	int		height;
	int		maxiter;
	int		i;
	int		j;
}			t_gradient_params;

typedef struct s_gradient_calc_params
{
	int					n;
	double				r;
	double				s;
	double				d;
	double _Complex		c;
}						t_gradient_calc_params;

typedef struct s_process_vars
{
	double	c_real;
	double	c_imag;
	int		samples;
	int		count;
}			t_process_vars;

/* ************************************************************************** */
/*                               FUNCTION PROTOTYPES                          */
/* ************************************************************************** */

/* color_palette_2.c*/
double _Complex			m_dwell_gradient(t_gradient_calc_params params);

/* main.c */
int						main(int argc, char **argv);

/* init.c */
void					init_data(t_data *data);
void					init_data_colors(t_data *data);
void					init_data_dimensions(t_data *data);
void					init_data_params(t_data *data);
void					init_fractal_params(t_data *data, int iter_count);

/* parser.c */
void					parse_arguments(int argc, char **argv, t_data *data);

/* mlx_setup.c */
void					initialize_mlx(t_data *data);
void					reset_view(t_data *data);
void					exit_fractol(t_data *data);

/* event_handlers.c */
int						deal_key(int key, t_data *data);
int						mouse_move(int x, int y, t_data *data);
int						mouse_release(int button, int x, int y, t_data *data);

/* mouse_event_handler */
void					handle_selection(t_mouse_data *m_data, t_data *data);
void					calculate_mouse_coordinates(t_mouse_data *m_data,
							t_data *data);
void					update_zoom_offset(t_mouse_data *m_data, t_data *data);
int						mouse_press(int button, int x, int y, t_data *data);

/* key_handlers_1.c */
void					adjust_fractal_parameters(int key, t_data *data);
void					handle_reset_and_exit(int key, t_data *data);
void					handle_movement(int key, t_data *data);
void					handle_zoom(int key, t_data *data);
void					handle_iter_adjustment(int key, t_data *data);

/* key_handlers_2.c */
void					handle_fast_mode(t_data *data);
void					handle_overlay(t_data *data);
void					handle_screen_mode(t_data *data);
void					enter_fullscreen(t_data *data);
void					exit_fullscreen(t_data *data);

/* color_handlers.c */
void					switch_palette_next(t_data *data);
void					switch_palette_prev(t_data *data);
void					adjust_c_imag(t_data *data, double delta);
void					adjust_c_real(t_data *data, double delta);

/* color_handlers_2.c */
void					reset_base_color_component(t_data *data, int key);

/* adjust_rgb.c*/
void					adjust_red(t_data *data, int key);
void					adjust_green(t_data *data, int key);
void					adjust_blue(t_data *data, int key);
void					adjust_base_color_component(t_data *data, int key);

/* drawing.c */
int						render_next_frame(t_data *data);
void					draw_fractal_with_iter(t_data *data, int iter_count);
void					draw_fractal(t_data *data, int iter_count);
void					*thread_draw_fractal(void *arg);
void					draw_fractal_line(t_data *data,
							int y, t_fractal_params *params);

/* color_utils_1.c */
int						get_color(t_color_args *args);
int						get_color_part1(t_color_args *args);
int						get_color_part2(t_color_args *args);
int						get_color_part3(t_color_args *args);
void					put_pixel(t_data *data, int x, int y, int color);

/* color_utils_2.c */
void					hsv_to_rgb(double h, int *r, int *g, int *b);
void					lch_to_rgb(t_lch_color *lch, int *r, int *g, int *b);
void					yuv_to_rgb(t_color_interior vars,
							int *r, int *g, int *b);
void					hsv2rgb(t_gradient_data gd);

/* color_utils_3.c */
void					calculate_magnitude(double z_real, double z_imag,
							double *magnitude_sq, double *magnitude_val);
double					calculate_angle(double z_real, double z_imag);
void					calculate_uv_components(double angle, double saturation,
							double *u, double *v);
int						apply_base_color(t_data *data, int r, int g, int b);

/* color_utils_4.c */
void					calculate_hsv_components(t_gradient_data gd, float *p,
							float *q, float *t);
void					get_rgb_from_case(t_hsv_to_rgb hsv,
							float *r, float *g, float *b);
void					handle_case_0_to_2(t_hsv_to_rgb hsv,
							float *r, float *g, float *b);
void					handle_case_3_to_5(t_hsv_to_rgb hsv,
							float *r, float *g, float *b);
void					assign_rgb_values(t_hsv_to_rgb hsv,
							float *red, float *grn, float *blu);

/* color_utils_5.c */
void					apply_rgb_values(t_hsv hsv, int *r, int *g, int *b);
void					set_rgb_case_012(t_hsv hsv, int *r, int *g, int *b);
void					set_rgb_case_345(t_hsv hsv, int *r, int *g, int *b);
void					scale_rgb_values(int *r, int *g, int *b);

/* color_palettes_1.c */
int						get_color_fire(int iter, int max_iter, t_data *data);
void					limit_color_values(int *r, int *g, int *b);
int						get_color_stripes(int iter, t_data *data);
int						get_color_smooth(int iter, t_data *data);
int						get_color_classic(int iter, t_data *data);

/* color				_palettes_2.c */
int						get_color_hsv(int iter, int max_iter, t_data *data);
int						get_color_gradient(int iter, int max_iter,
							t_data *data);
int						get_color_black_white(int iter, t_data *data);
int						get_color_escape_time(int iter, int max_iter,
							t_data *data);
int						get_color_continuous_potential(t_color_args *args);

/* color_palettes_3.c */
int						get_color_interior_distance(t_color_args *args);
int						get_color_lch(int iter, int max_iter, t_data *data);
int						get_color_exp_cyclic_lch_no_shading(int iter,
							int max_iter, t_data *data);
int						get_color_exp_cyclic_lch_shading(int iter,
							int max_iter, t_data *data);
int						get_color_derivative_bailout(int iter, double z_real,
							double z_imag, t_data *data);

/* color_palettes_4.c */
int						get_color_custom_interior(t_color_args *args);
void					dwell_gradient(t_gradient_params params,
							const int *counts,
							unsigned char *pixel, t_data *data);
int						get_color_dwell_gradient(t_color_args *args);
double					m_continuous_dwell(int N, double R, double _Complex c);

/* color_palettes_5.c */
void					set_color(t_color *color, float value);
t_gradient_calc_params	init_calc_params(int maxiter, double R,
							double _Complex c);
void					process_gradient(t_gradient_data *gd,
							t_gradient_calc_params params,
							const int *counts, t_gradient_params gparams);
void					apply_base_color_dwell(t_color *color, t_data *data);
void					set_pixel_colors(unsigned char *pixel,
							int k, t_color *color);

/* color_palettes_6.c */
void					init_gradient_data(t_gradient_data *gd,
							t_gradient_params params);
int						get_color_logarithmic(int iter,
							int max_iter, t_data *data);
/* threads_init.c*/
void					init_thread_data(t_data *thread_data, t_data *data,
							int height_per_thread, int i);
void					cleanup_threads(pthread_t *threads, int i);

/* ui_drawing_1.c */
void					draw_info_strings(t_data *data);
void					draw_controls(t_data *data, int *y);
void					draw_parameters(t_data *data, int *y);
void					draw_zoom_info(t_data *data, int *y);
void					draw_iter_info(t_data *data, int *y);

/* ui_drawing_2.c */
void					draw_rgb_info(t_data *data, int *y);
char					*create_rgb_string(t_data *data);
void					draw_fractal_type(t_data *data, int *y);
void					draw_palette_type(t_data *data, int *y);
void					draw_selection_rectangle(t_data *data);

/* selection.c */
void					get_sorted_selection(t_data *data, t_zoom *zoom);
void					draw_rectangle_edges(t_data *data, t_draw_rect *rect);
void					swap_int(int *a, int *b);
void					zoom_to_selection(t_data *data);
void					calculate_zoom_and_offset(t_data *data, t_zoom zoom);

/* compute.c */
int						compute_fractal(t_data *data,
							t_fractal_vars *vars, int iter_count);

/* julia.c */
int						compute_julia(t_data *data,
							t_fractal_vars *vars, int iter_count);

/* mandelbrot.c */
int						compute_mandelbrot(t_data *data,
							t_fractal_vars *vars, int iter_count);
void					init_mandel_vars(t_fractal_vars *vars);
int						compute_tricorn(t_fractal_vars *vars, int iter_count);
int						compute_standard(t_fractal_vars *vars, int iter_count);

/* burning_ship.c */
int						compute_burning_ship(t_fractal_vars *vars,
							int iter_count);

/* lyapunov.c */
void					init_lyap_vars(t_lyap_vars *vars, int iter_count);
int						check_value_bounds(double value, double r);
int						calculate_final_value(double sum_log_deriv, int iter);
double					get_r_value(t_fractal_vars *vars, char sequence_char);
int						compute_lyapunov(t_fractal_vars *vars, int iter_count);

/* newton.c */
int						compute_newton(t_fractal_vars *vars, int iter_count);
void					init_newton_vars(t_fractal_vars *vars);
double					calculate_denominator(double old_real, double old_imag);
void					calculate_next_z(t_fractal_vars *vars, double old_real,
							double old_imag, double denominator);
int						check_convergence(double old_real, double old_imag,
							double new_real, double new_imag);

/* buddhabrot_1.c */
void					render_buddhabrot(t_data *data);
int						init_main_histogram(t_data *data);
int						init_thread_resources(t_thread_data **thread_data,
							pthread_t **threads);
int						init_thread_data_arrays(t_thread_data *thread_data,
							t_data *data, int i);
int						create_and_run_threads(t_thread_data *thread_data,
							pthread_t *threads, t_data *data);

/* buddhabrot_2.c */
void					render_buddhabrot_image(t_data *data);
unsigned int			find_max_value(unsigned int *array, int size);
void					cleanup_buddhabrot(t_data *data, pthread_t *threads,
							t_thread_data *thread_data);
void					*thread_generate_buddhabrot(void *arg);
void					*process_buddhabrot_section(void *arg);

/* buddhabrot_3.c */
void					merge_local_histograms(t_data *data,
							t_thread_data *thread_data,
							int num_threads);
void					process_point(t_data *data,
							double c_real, double c_imag);
void					update_histogram(t_data *data, double *traj_real,
							double *traj_imag, int length);
void					normalize_and_render_buddhabrot(t_data *data);
void					process_buddhabrot_point(t_data *data,
							double c_real, double c_imag);

/* buddhabrot_4.c */
int						allocate_trajectories(t_data *data, double **traj_real,
							double **traj_imag);
unsigned int			find_max_histogram_value(t_data *data);
void					render_line(t_data *data, int y,
							unsigned int max_value);
void					process_trajectory(t_data *data, double *traj_real,
							double *traj_imag, int iter);
int						calculate_color(unsigned int value,
							unsigned int max_value);

/* buddhabrot_4.c */
void					init_and_check_point(double *z_real, double *z_imag,
							double c_real, double c_imag);

/* utils_1.c */
void					ft_swap(int *a, int *b);
int						ft_clamp(int value, int min, int max);
char					*ft_ftoa(double n, int precision);
char					*ft_lltoa(long long n, long long nb);
int						ft_numlen(long long n);

/* utils_2.c */
char					*str_join_free(char *s1, char *s2);
int						check_escape(double z_real, double z_imag);
void					calculate_next_point(double *z_real, double *z_imag,
							double c_real, double c_imag);
int						is_in_main_cardioid(double x, double y);
int						is_in_period2_bulb(double x, double y);

/* utils_3.c */
void					update_histogram_point(t_data *data,
							int screen_x, int screen_y);
void					calculate_screen_coords(double real, double imag,
							int *screen_x, int *screen_y);
void					move_offset(t_data *data, double x_factor,
							double y_factor);
void					handle_zoom(int key, t_data *data);
void					calculate_buddha_bounds(t_data *data, t_zoom *zoom);

/* utils_4.c */
void					calculate_coordinates(t_data *data, t_zoom *zoom);
void					update_zoom_and_center(t_data *data, t_zoom *zoom);
void					update_zoom_and_offset(t_data *data, t_zoom zoom);
int						handle_allocation(char **int_str, char **frac_str,
							long long int_part, int precision);
char					*join_and_free(char *int_str, char *frac_str);

/* utils_5.c */
void					process_fractional_part(char *frac_str,
							double frac_part, int precision);

#endif