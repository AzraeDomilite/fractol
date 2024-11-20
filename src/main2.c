/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:32:49 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:33:42 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

pthread_mutex_t	g_histogram_mutex = PTHREAD_MUTEX_INITIALIZER;
const double pi = 3.141592653589793;

static void	init_data_colors(t_data *data)
{
	data->palette_type = PALETTE_CUSTOM_INTERIOR;
	data->base_color.r = MAX_COLOR_VALUE;
	data->base_color.g = MAX_COLOR_VALUE;
	data->base_color.b = MAX_COLOR_VALUE;
	data->line_brightness = 0.0;
	data->cell_brightness = 0.75;
	data->base_saturation = 0.25;
}

static void	init_data_dimensions(t_data *data)
{
	data->buddha_real_min = BUDDHA_REAL_MIN;
	data->buddha_real_max = BUDDHA_REAL_MAX;
	data->buddha_imag_min = BUDDHA_IMAG_MIN;
	data->buddha_imag_max = BUDDHA_IMAG_MAX;
	data->original_width = WIN_WIDTH;
	data->original_height = WIN_HEIGHT;
	data->width = WIN_WIDTH;
	data->height = WIN_HEIGHT;
}

static void	init_data_params(t_data *data)
{
	data->branch_factor = 2.0;
	data->branch_offset = 1.0;
	data->branch_base = 3.0;
	data->line_width_base = 0.25;
	data->overlay_enabled = 1;
	data->fast_mode = 1;
	data->is_fullscreen = 0;
}

void	init_data(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
	data->zoom = 1.0;
	data->c_real = INITIAL_C_REAL;
	data->c_imag = INITIAL_C_IMAG;
	data->redraw = 1;
	data->max_iter = DEFAULT_PREVIEW_ITER;
	data->fractal_type = JULIA;
	init_data_colors(data);
	init_data_dimensions(data);
	init_data_params(data);
	if (pthread_mutex_init(&data->histogram_mutex, NULL) != 0)
	{
		ft_printf(ERROR_MUTEX_INIT);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	parse_arguments(argc, argv, &data);
	initialize_mlx(&data);
	reset_view(&data);
	mlx_hook(data.win, 2, 1L << 0, deal_key, &data);
	mlx_hook(data.win, 17, 0, (int (*)())exit_fractol, &data);
	mlx_mouse_hook(data.win, mouse_press, &data);
	mlx_hook(data.win, 6, 1L << 6, mouse_move, &data);
	mlx_hook(data.win, 5, 1L << 3, mouse_release, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);
	ft_printf(ERROR_MSG_LOOP);
	exit(EXIT_FAILURE);
}

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
	else if (ft_strcmp(argv[1], "buddhabrot") == 0)
		data->fractal_type = BUDDHABROT;
	else
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
}

void	initialize_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		ft_printf(ERROR_MLX_INIT);
		exit(EXIT_FAILURE);
	}
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
}

void	reset_view(t_data *data)
{
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->c_real = INITIAL_C_REAL;
	data->c_imag = INITIAL_C_IMAG;
	data->redraw = 1;
	data->fast_mode = 0;
	data->max_iter = DEFAULT_PREVIEW_ITER;
	data->base_color.r = MAX_COLOR_VALUE;
	data->base_color.g = MAX_COLOR_VALUE;
	data->base_color.b = MAX_COLOR_VALUE;
	data->is_selecting = 0;
}

void	exit_fractol(t_data *data)
{
	pthread_mutex_destroy(&data->histogram_mutex);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	exit(EXIT_SUCCESS);
}

static void	handle_fast_mode(t_data *data)
{
	data->fast_mode = !data->fast_mode;
	data->redraw = 1;
	if (data->fast_mode)
		ft_printf("Mode Rapide Activé\n");
	else
		ft_printf("Mode Rapide Désactivé\n");
}

static void	handle_overlay(t_data *data)
{
	data->overlay_enabled = !data->overlay_enabled;
	data->redraw = 1;
	if (data->overlay_enabled)
		ft_printf("Overlay Activé\n");
	else
		ft_printf("Overlay Désactivé\n");
}

static void	handle_screen_mode(t_data *data)
{
	if (data->is_fullscreen)
		exit_fullscreen(data);
	else
		enter_fullscreen(data);
	data->redraw = 1;
}

int	deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);
	if (key == K_1)
		handle_fast_mode(data);
	if (key == K_H)
		handle_overlay(data);
	if (key == K_0)
		handle_screen_mode(data);
	return (0);
}

void	enter_fullscreen(t_data *data)
{
	if (data->is_fullscreen)
		return ;
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	data->win = mlx_new_window(data->mlx, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT, "Fract'ol - Plein Écran");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	data->width = FULLSCREEN_WIDTH;
	data->height = FULLSCREEN_HEIGHT;
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->is_fullscreen = 1;
	render_next_frame(data);
}

void	exit_fullscreen(t_data *data)
{
	if (!data->is_fullscreen)
		return ;
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	data->win = mlx_new_window(data->mlx, data->original_width, data->original_height, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, data->original_width, data->original_height);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	data->width = data->original_width;
	data->height = data->original_height;
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->is_fullscreen = 0;
	render_next_frame(data);
}

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

void	handle_reset_and_exit(int key, t_data *data)
{
	if (key == K_R)
	{
		reset_view(data);
		data->redraw = 1;
	}
	else if (key == K_ESC || key == K_Q)
		exit_fractol(data);
}

void	handle_movement(int key, t_data *data)
{
	if (key == K_S || key == K_DOWN)
		move_offset(data, 0, MOVE_FACTOR);
	else if (key == K_W || key == K_UP)
		move_offset(data, 0, -MOVE_FACTOR);
	else if (key == K_D || key == K_RIGHT)
		move_offset(data, MOVE_FACTOR, 0);
	else if (key == K_A || key == K_LEFT)
		move_offset(data,- MOVE_FACTOR, 0);
}

void	move_offset(t_data *data, double x_factor, double y_factor)
{
	if (data->fractal_type == BUDDHABROT)
	{
		double x_shift = x_factor * (data->buddha_real_max - data->buddha_real_min);
		double y_shift = y_factor * (data->buddha_imag_max - data->buddha_imag_min);
		data->buddha_real_min += x_shift;
		data->buddha_real_max += x_shift;
		data->buddha_imag_min += y_shift;
		data->buddha_imag_max += y_shift;
	}
	else
	{
		double x_shift = x_factor * (4.0 / data->zoom);
		double y_shift = y_factor * (3.0 / data->zoom);
		data->offset_x += x_shift;
		data->offset_y += y_shift;
	}
	data->redraw = 1;
}

void handle_zoom(int key, t_data *data)
{
	double zoom_factor = (key == K_NUM_PLUS) ? ZOOM_FACTOR : 1.0 / ZOOM_FACTOR;
	double center_x = (data->buddha_real_min + data->buddha_real_max) / 2.0;
	double center_y = (data->buddha_imag_min + data->buddha_imag_max) / 2.0;
	double width = (data->buddha_real_max - data->buddha_real_min) / zoom_factor;
	double height = (data->buddha_imag_max - data->buddha_imag_min) / zoom_factor;

	data->buddha_real_min = center_x - width / 2.0;
	data->buddha_real_max = center_x + width / 2.0;
	data->buddha_imag_min = center_y - height / 2.0;
	data->buddha_imag_max = center_y + height / 2.0;
	data->redraw = 1;
}


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

void	switch_palette_next(t_data *data)
{
	if (data->palette_type + 1 >= PALETTE_COUNT)
		data->palette_type = 0;
	else
		data->palette_type++;
	data->redraw = 1;
}

void	switch_palette_prev(t_data *data)
{
	if (data->palette_type == 0)
		data->palette_type = PALETTE_COUNT - 1;
	else
		data->palette_type--;
	data->redraw = 1;
}

void	adjust_c_imag(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_imag += delta;
	else
		data->offset_y += delta;
	data->redraw = 1;
}

void	adjust_c_real(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_real += delta;
	else
		data->offset_x += delta;
	data->redraw = 1;
}

void	adjust_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_7)
		ADJUST_COLOR_COMPONENT(data->base_color.r, -COLOR_ADJUST_STEP);
	else if (key == K_NUM_9)
		ADJUST_COLOR_COMPONENT(data->base_color.r, COLOR_ADJUST_STEP);
	else if (key == K_NUM_4)
		ADJUST_COLOR_COMPONENT(data->base_color.g, -COLOR_ADJUST_STEP);
	else if (key == K_NUM_6)
		ADJUST_COLOR_COMPONENT(data->base_color.g, COLOR_ADJUST_STEP);
	else if (key == K_NUM_1)
		ADJUST_COLOR_COMPONENT(data->base_color.b, -COLOR_ADJUST_STEP);
	else if (key == K_NUM_3)
		ADJUST_COLOR_COMPONENT(data->base_color.b, COLOR_ADJUST_STEP);
	data->redraw = 1;
}

void	reset_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_8)
		data->base_color.r = MAX_COLOR_VALUE;
	else if (key == K_NUM_5)
		data->base_color.g = MAX_COLOR_VALUE;
	else if (key == K_NUM_2)
		data->base_color.b = MAX_COLOR_VALUE;
	data->redraw = 1;
}

double m_continuous_dwell(int N, double R, double _Complex c)
{
	int iter;
	double _Complex z = 0 + 0*I;

	for (iter = 0; iter < N; iter++)
	{
		z = cpow(z, 2) + c;
		if (cabs(z) > R)
			break ;
	}
	return (double)iter;
}


double _Complex	m_dwell_gradient(int N, double R, double s, double d, double _Complex c)
{
	double m  = m_continuous_dwell(N, R, c);
	double mx = m_continuous_dwell(N, R, c + d);
	double my = m_continuous_dwell(N, R, c + d * I);
	double vx = m - mx;
	double vy = m - my;
	double vz = s;
	double vm = sqrt(vx * vx + vy * vy + vz * vz);
	return vz / vm;
}

int	mouse_press(int button, int x, int y, t_data *data)
{
	double	mouse_re;
	double	mouse_im;
	double	zoom_factor;

	if (button == 1)
	{
		data->is_selecting = 1;
		data->select_start_x = x;
		data->select_start_y = y;
		data->select_end_x = x;
		data->select_end_y = y;
	}
	else if (button == 4 || button == 5)
	{
		mouse_re = (x / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom + data->offset_x;
		mouse_im = (y / (double)WIN_HEIGHT - 0.5) * 3.0 / data->zoom + data->offset_y;
		zoom_factor = (button == 4) ? ZOOM_FACTOR : 1.0 / ZOOM_FACTOR;
		data->zoom *= zoom_factor;

		data->offset_x = mouse_re - (x / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom;
		data->offset_y = mouse_im - (y / (double)WIN_HEIGHT - 0.5) * 3.0 / data->zoom;
		data->redraw = 1;
	}
	return (0);
}

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

void	draw_fractal_with_iter(t_data *data, int iter_count)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, data->width, data->height);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	if (data->fractal_type == BUDDHABROT)
	{
		data->max_iter = iter_count;
		render_buddhabrot(data);
	}
	else
	{
		draw_fractal(data, iter_count);
	}
	if (data->is_selecting)
		draw_selection_rectangle(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	draw_info_strings(data);
}

static void	init_fractal_params(t_data *data, int iter_count)
{
	data->scale = 1.0 / (data->zoom);
	data->iter_count = iter_count;
	if (data->fractal_type == JULIA)
	{
		data->real_min = -2.0 / data->zoom + data->offset_x;
		data->imag_min = -2.0 / data->zoom + data->offset_y;
	}
	else
	{
		data->real_min = -2.5 / data->zoom + data->offset_x;
		data->imag_min = -1.5 / data->zoom + data->offset_y;
	}
}

static void	init_thread_data(t_data *thread_data, t_data *data,
		int height_per_thread, int i)
{
	thread_data[i] = *data;
	thread_data[i].y_start = i * height_per_thread;
	thread_data[i].y_end = (i + 1) * height_per_thread;
	if (i == NUM_THREADS - 1)
		thread_data[i].y_end = WIN_HEIGHT;
}

static void	cleanup_threads(pthread_t *threads, int i)
{
	while (--i >= 0)
		pthread_join(threads[i], NULL);
}

void	draw_fractal(t_data *data, int iter_count)
{
	pthread_t	threads[NUM_THREADS];
	t_data		thread_data[NUM_THREADS];
	int			i;
	int			height_per_thread;

	init_fractal_params(data, iter_count);
	height_per_thread = WIN_HEIGHT / NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		init_thread_data(thread_data, data, height_per_thread, i);
		if (pthread_create(&threads[i], NULL, thread_draw_fractal,
				&thread_data[i]) != 0)
		{
			ft_printf(ERROR_THREAD_CREATE);
			cleanup_threads(threads, i);
			exit_fractol(data);
		}
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
}

void	*thread_draw_fractal(void *arg)
{
	t_data			*data;
	t_fractal_params	params;
	int				y;

	data = (t_data *)arg;
	params.real_min = data->real_min;
	params.imag_min = data->imag_min;
	params.scale = data->scale;
	params.iter_count = data->iter_count;
	y = data->y_start;
	while (y < data->y_end)
	{
		draw_fractal_line(data, y, &params);
		y++;
	}
	return (NULL);
}

void	draw_fractal_line(t_data *data, int y, t_fractal_params *params)
{
	t_fractal_vars	vars;

	vars.x = 0;
	while (vars.x < WIN_WIDTH)
	{
		vars.c_real = params->real_min + ((double)vars.x / WIN_WIDTH) * 4.0 * params->scale;
		vars.c_imag = params->imag_min + ((double)y / WIN_HEIGHT) * 3.0 * params->scale;	
		vars.iter = compute_fractal(data, &vars, params->iter_count);
		put_pixel(data, vars.x, y, get_color(vars.iter, 
				data, vars.z_real, vars.z_imag, data->max_iter));
		vars.x++;
	}
}

static int	init_main_histogram(t_data *data)
{
	data->histogram = (unsigned int *)ft_calloc(WIN_WIDTH * WIN_HEIGHT,
			sizeof(unsigned int));
	if (!data->histogram)
	{
		ft_printf(ERROR_MALLOC_HIST);
		return (0);
	}
	return (1);
}

static int	init_thread_resources(t_thread_data **thread_data,
		pthread_t **threads)
{
	*threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	*thread_data = (t_thread_data *)malloc(NUM_THREADS * sizeof(t_thread_data));
	if (!(*threads) || !(*thread_data))
		return (0);
	return (1);
}

static int	init_thread_data_arrays(t_thread_data *thread_data,
		t_data *data, int i)
{
	thread_data[i].data = data;
	thread_data[i].seed = i + 1;
	thread_data[i].samples = SAMPLES_PER_THREAD / NUM_THREADS;
	thread_data[i].traj_real = malloc(data->max_iter * sizeof(double));
	thread_data[i].traj_imag = malloc(data->max_iter * sizeof(double));
	thread_data[i].local_histogram = (unsigned int *)ft_calloc(WIN_WIDTH
			* WIN_HEIGHT, sizeof(unsigned int));
	if (!thread_data[i].traj_real || !thread_data[i].traj_imag
		|| !thread_data[i].local_histogram)
		return (0);
	return (1);
}

static int	create_and_run_threads(t_thread_data *thread_data, pthread_t *threads, t_data *data)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		if (!init_thread_data_arrays(thread_data, data, i))
		{
			cleanup_buddhabrot(data, threads, thread_data);
			ft_printf(ERROR_MALLOC_THREAD_DATA);
			return (0);
		}
		if (pthread_create(&threads[i], NULL, process_buddhabrot_section,
				&thread_data[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(threads[i], NULL);
			cleanup_buddhabrot(data, threads, thread_data);
			ft_printf(ERROR_THREAD_CREATE);
			return (0);
		}
		i++;
	}
	return (1);
}

void	render_buddhabrot(t_data *data)
{
	t_thread_data	*thread_data;
	pthread_t		*threads;
	int				i;

	if (!init_main_histogram(data))
		exit_fractol(data);
	if (!init_thread_resources(&thread_data, &threads))
	{
		free(data->histogram);
		ft_printf(ERROR_MALLOC_THREAD);
		exit_fractol(data);
	}
	if (!create_and_run_threads(thread_data, threads, data))
		exit_fractol(data);
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	merge_local_histograms(data, thread_data, NUM_THREADS);
	render_buddhabrot_image(data);
	cleanup_buddhabrot(data, threads, thread_data);
}

void	merge_local_histograms(t_data *data, t_thread_data *thread_data, int num_threads)
{
	int	i;
	int	j;

	for (i = 0; i < num_threads; i++)
	{
		for (j = 0; j < WIN_WIDTH * WIN_HEIGHT; j++)
		{
			if (thread_data[i].local_histogram[j] > 0)
			{
				pthread_mutex_lock(&data->histogram_mutex);
				data->histogram[j] += thread_data[i].local_histogram[j];
				pthread_mutex_unlock(&data->histogram_mutex);
			}
		}
	}
}


void	cleanup_buddhabrot(t_data *data, pthread_t *threads, t_thread_data *thread_data)
{
	int	i;

	if (thread_data)
	{
		for (i = 0; i < NUM_THREADS; i++)
		{
			if (thread_data[i].traj_real)
				free(thread_data[i].traj_real);
			if (thread_data[i].traj_imag)
				free(thread_data[i].traj_imag);
			if (thread_data[i].local_histogram)
				free(thread_data[i].local_histogram);
		}
		free(thread_data);
	}
	if (threads)
		free(threads);
	if (data->histogram)
	{
		free(data->histogram);
		data->histogram = NULL;
	}
}


void render_buddhabrot_image(t_data *data)
{
	unsigned int max_value;
	double normalized;
	int i;
	int color;

	max_value = find_max_value(data->histogram, WIN_WIDTH * WIN_HEIGHT);
	if (max_value == 0)
		return;
	i = 0;
	while (i < WIN_WIDTH * WIN_HEIGHT)
	{
		normalized = log(1 + data->histogram[i]) / log(1 + max_value);
		color = (int)(normalized * MAX_COLOR_VALUE);
		put_pixel(data, i % WIN_WIDTH, i / WIN_WIDTH,
			(color << 16) | (color << 8) | color);
		i++;
	}
}


unsigned int	find_max_value(unsigned int *array, int size)
{
	unsigned int	max;
	int			i;

	max = 0;
	i = 0;
	while (i < size)
	{
		if (array[i] > max)
			max = array[i];
		i++;
	}
	return (max);
}
void	*thread_generate_buddhabrot(void *arg)
{
	t_thread_data	*thread;
	t_data			*data;
	double			c_real;
	double			c_imag;
	int				i;

	thread = (t_thread_data *)arg;
	data = thread->data;
	srand(thread->seed);
	i = 0;
	while (i < thread->samples)
	{
		c_real = ((double)rand() / RAND_MAX) * 3.0 - 2.0;
		c_imag = ((double)rand() / RAND_MAX) * 3.0 - 1.5;
		process_buddhabrot_point(data, c_real, c_imag);
		i++;
	}
	return (NULL);
}

void *process_buddhabrot_section(void *arg)
{
    t_thread_data *thread = (t_thread_data *)arg;
    int i;
    double c_real;
    double c_imag;
    int samples_per_thread;
    t_data *data = thread->data;

    samples_per_thread = SAMPLES_PER_THREAD;
    srand(thread->seed);
    for (i = 0; i < samples_per_thread; i++)
    {
        c_real = data->buddha_real_min + ((double)rand() / RAND_MAX)
                * (data->buddha_real_max - data->buddha_real_min);
        c_imag = data->buddha_imag_min + ((double)rand() / RAND_MAX)
                * (data->buddha_imag_max - data->buddha_imag_min);
        process_point(data, c_real, c_imag);
    }
    return NULL;
}



static void	update_histogram_point(t_data *data, int screen_x, int screen_y)
{
	if (screen_x >= 0 && screen_x < WIN_WIDTH
		&& screen_y >= 0 && screen_y < WIN_HEIGHT)
	{
		pthread_mutex_lock(&data->histogram_mutex);
		data->histogram[screen_y * WIN_WIDTH + screen_x]++;
		pthread_mutex_unlock(&data->histogram_mutex);
	}
}

static void	calculate_screen_coords(double real, double imag,
		int *screen_x, int *screen_y)
{
	*screen_x = (int)((real - BUDDHA_REAL_MIN)
			/ (BUDDHA_REAL_MAX - BUDDHA_REAL_MIN) * WIN_WIDTH);
	*screen_y = (int)((imag - BUDDHA_IMAG_MIN)
			/ (BUDDHA_IMAG_MAX - BUDDHA_IMAG_MIN) * WIN_HEIGHT);
}

static void	process_trajectory(t_data *data, double *traj_real,
		double *traj_imag, int iter)
{
	int	i;
	int	screen_x;
	int	screen_y;

	i = 0;
	while (i < iter)
	{
		calculate_screen_coords(traj_real[i], traj_imag[i],
			&screen_x, &screen_y);
		update_histogram_point(data, screen_x, screen_y);
		i++;
	}
}

void	process_buddhabrot_point(t_data *data, double c_real, double c_imag)
{
	double	z_real;
	double	z_imag;
	double	tmp;
	double	trajectory_real[MAX_ITER];
	double	trajectory_imag[MAX_ITER];
	int		iter;

	if (is_in_main_cardioid(c_real, c_imag)
		|| is_in_period2_bulb(c_real, c_imag))
		return ;
	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	while (iter < data->max_iter)
	{
		trajectory_real[iter] = z_real;
		trajectory_imag[iter] = z_imag;
		tmp = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp;
		if (z_real * z_real + z_imag * z_imag > 4.0)
		{
			process_trajectory(data, trajectory_real, trajectory_imag, iter);
			break ;
		}
		iter++;
	}
}

static int	allocate_trajectories(t_data *data, double **traj_real,
		double **traj_imag)
{
	*traj_real = malloc(data->max_iter * sizeof(double));
	*traj_imag = malloc(data->max_iter * sizeof(double));
	if (!(*traj_real) || !(*traj_imag))
	{
		free(*traj_real);
		free(*traj_imag);
		return (0);
	}
	return (1);
}

static int	check_escape(double z_real, double z_imag)
{
	return (z_real * z_real + z_imag * z_imag > ESCAPE_RADIUS);
}

static void	calculate_next_point(double *z_real, double *z_imag,
		double c_real, double c_imag)
{
	double	tmp;

	tmp = *z_real * *z_real - *z_imag * *z_imag + c_real;
	*z_imag = 2.0 * *z_real * *z_imag + c_imag;
	*z_real = tmp;
}

void	process_point(t_data *data, double c_real, double c_imag)
{
	double	*traj_real;
	double	*traj_imag;
	double	z_real;
	double	z_imag;
	int		iter;

	if (!allocate_trajectories(data, &traj_real, &traj_imag))
		return ;
	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	while (iter < data->max_iter)
	{
		traj_real[iter] = z_real;
		traj_imag[iter] = z_imag;
		calculate_next_point(&z_real, &z_imag, c_real, c_imag);
		if (check_escape(z_real, z_imag))
		{
			update_histogram(data, traj_real, traj_imag, iter);
			break ;
		}
		iter++;
	}
	free(traj_real);
	free(traj_imag);
}

void update_histogram(t_data *data, double *traj_real,
		double *traj_imag, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		int x = (int)((traj_real[i] - BUDDHA_REAL_MIN)
				/ (BUDDHA_REAL_MAX - BUDDHA_REAL_MIN) * WIN_WIDTH);
		int y = (int)((traj_imag[i] - BUDDHA_IMAG_MIN)
				/ (BUDDHA_IMAG_MAX - BUDDHA_IMAG_MIN) * WIN_HEIGHT);
		if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		{
			pthread_mutex_lock(&data->histogram_mutex);
			data->histogram[y * WIN_WIDTH + x]++;
			pthread_mutex_unlock(&data->histogram_mutex);
		}
	}
}




static unsigned int	find_max_histogram_value(t_data *data)
{
	unsigned int	max_value;
	int			i;

	max_value = 0;
	i = 0;
	while (i < WIN_WIDTH * WIN_HEIGHT)
	{
		if (data->histogram[i] > max_value)
			max_value = data->histogram[i];
		i++;
	}
	return (max_value);
}

static int	calculate_color(unsigned int value, unsigned int max_value)
{
	double	normalized;
	int		color;

	normalized = log(1 + value) / log(1 + max_value);
	color = (int)(normalized * 255);
	return ((color << 16) | (color << 8) | color);
}

static void	render_line(t_data *data, int y, unsigned int max_value)
{
	int				x;
	unsigned int	value;
	int				color;

	x = 0;
	while (x < WIN_WIDTH)
	{
		value = data->histogram[y * WIN_WIDTH + x];
		color = calculate_color(value, max_value);
		put_pixel(data, x, y, color);
		x++;
	}
}

void	normalize_and_render_buddhabrot(t_data *data)
{
	unsigned int	max_value;
	int			y;

	max_value = find_max_histogram_value(data);
	if (max_value == 0)
		return ;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		render_line(data, y, max_value);
		y++;
	}
}

int	is_in_main_cardioid(double x, double y)
{
	double	p;

	p = sqrt((x - 0.25) * (x - 0.25) + y * y);
	return (x <= p - 2.0 * p * p + 0.25);
}

int	is_in_period2_bulb(double x, double y)
{
	double	squared;

	squared = (x + 1.0) * (x + 1.0) + y * y;
	return (squared <= 0.0625);
}

static int	get_color_part1(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_FIRE)
		return (get_color_fire(iter, max_iter, data));
	if (data->palette_type == PALETTE_STRIPES)
		return (get_color_stripes(iter, data));
	if (data->palette_type == PALETTE_SMOOTH)
		return (get_color_smooth(iter, data));
	if (data->palette_type == PALETTE_CLASSIC)
		return (get_color_classic(iter, data));
	if (data->palette_type == PALETTE_DERIVATIVE_BAILOUT)
		return (get_color_derivative_bailout(iter, z_real, z_imag, data));
	if (data->palette_type == PALETTE_LOGARITHMIC)
		return (get_color_logarithmic(iter, max_iter, data));
	return (0);
}

static int	get_color_part2(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_HSV)
		return (get_color_hsv(iter, max_iter, data));
	if (data->palette_type == PALETTE_GRADIENT)
		return (get_color_gradient(iter, max_iter, data));
	if (data->palette_type == PALETTE_BLACK_WHITE)
		return (get_color_black_white(iter, data));
	if (data->palette_type == PALETTE_ESCAPE_TIME)
		return (get_color_escape_time(iter, max_iter, data));
	if (data->palette_type == PALETTE_CONTINUOUS_POTENTIAL)
		return (get_color_continuous_potential(iter, z_real,
				z_imag, max_iter, data));
	if (data->palette_type == PALETTE_INTERIOR_DISTANCE)
		return (get_color_interior_distance(iter, z_real,
				z_imag, max_iter, data));
	return (0);
}

static int	get_color_part3(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_LCH)
		return (get_color_lch(iter, max_iter, data));
	if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_NO_SHADING)
		return (get_color_exp_cyclic_lch_no_shading(iter, max_iter, data));
	if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_SHADING)
		return (get_color_exp_cyclic_lch_shading(iter, max_iter, data));
	if (data->palette_type == PALETTE_DWELL_GRADIENT)
		return (get_color_dwell_gradient(iter, max_iter, data,
				z_real, z_imag));
	if (data->palette_type == PALETTE_CUSTOM_INTERIOR)
		return (get_color_custom_interior(iter, max_iter, data,
				z_real, z_imag));
	return (0);
}

int	get_color(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	int	color;

	color = get_color_part1(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	color = get_color_part2(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	color = get_color_part3(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	return (COLOR_BLACK);
}

static void	assign_rgb_values(float r, float g, float b,
		float *red, float *grn, float *blu)
{
	*red = r;
	*grn = g;
	*blu = b;
}

static void	calculate_hsv_components(float h, float v, float s,
		float *p, float *q, float *t)
{
	float	f;

	f = h - floorf(h);
	*p = v * (1 - s);
	*q = v * (1 - (s * f));
	*t = v * (1 - (s * (1 - f)));
}

static void	get_rgb_from_case(int case_value, float v, float p,
		float q, float t, float *r, float *g, float *b)
{
	if (case_value == 0)
		(*r = v, *g = t, *b = p);
	else if (case_value == 1)
		(*r = q, *g = v, *b = p);
	else if (case_value == 2)
		(*r = p, *g = v, *b = t);
	else if (case_value == 3)
		(*r = p, *g = q, *b = v);
	else if (case_value == 4)
		(*r = t, *g = p, *b = v);
	else
		(*r = v, *g = p, *b = q);
}

void	hsv2rgb(float h, float s, float v, float *red, float *grn, float *blu)
{
	float	p;
	float	q;
	float	t;
	float	r;
	float	g;
	float	b;
	int		case_value;

	if (s == 0)
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		h = 6 * (h - floorf(h));
		case_value = (int)floorf(h);
		calculate_hsv_components(h, v, s, &p, &q, &t);
		get_rgb_from_case(case_value, v, p, q, t, &r, &g, &b);
	}
	assign_rgb_values(r, g, b, red, grn, blu);
}


void	dwell_gradient(int width, int height, int maxiter, int i, int j, const int *counts, unsigned char *pixel, t_data *data) 
{
	int	k0 = height * i + j;
	float	r = 0.0f, g = 0.0f, b = 0.0f;
	
	if (counts[k0] == maxiter)
	{
		r = g = b = 1.0f;
	}
	else
	{
		double _Complex c = (double)i + (double)j * I;
		double _Complex gradient = m_dwell_gradient(maxiter, 4.0, 1.0, 0.1, c);
		
		double vz = creal(gradient);
		float	hue = fmod((vz * 360.0), 360.0f) / 360.0f;
		float	saturation = 1.0f;
		float	value = (counts[k0] < maxiter) ? 1.0f : 0.0f;
		
		hsv2rgb(hue, saturation, value, &r, &g, &b);
		r = (r * data->base_color.r) / MAX_COLOR_VALUE;
		g = (g * data->base_color.g) / MAX_COLOR_VALUE;
		b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	}
	
	int	k = (width * j + i) * 3;
	pixel[k + 0] = fminf(fmaxf(255 * r, 0), 255);
	pixel[k + 1] = fminf(fmaxf(255 * g, 0), 255);
	pixel[k + 2] = fminf(fmaxf(255 * b, 0), 255);
}


int get_color_dwell_gradient(int iter, int max_iter, t_data *data, double z_real, double z_imag)
{
    double _Complex c = z_real + z_imag * I;
    double _Complex gradient = m_dwell_gradient(max_iter, ESCAPE_RADIUS, 1.0, 0.1, c);
    double vz = creal(gradient);
    double hue = fmod((vz * 360.0), 360.0);
    double saturation = 1.0;
    double value = (iter < max_iter) ? 1.0 : 0.0;
    float r, g, b;
    
    hsv2rgb((float)(hue / 360.0), (float)saturation, (float)value, &r, &g, &b);
    r = (r * data->base_color.r) / MAX_COLOR_VALUE;
    g = (g * data->base_color.g) / MAX_COLOR_VALUE;
    b = (b * data->base_color.b) / MAX_COLOR_VALUE;

    return ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
}



int	get_color_fire(int iter, int max_iter, t_data *data)
{
	double	t;
	int		r;
	int		g;
	int		b;

	if (iter == max_iter)
		return (COLOR_BLACK);
	t = (double)iter / max_iter;
	r = (int)(MAX_COLOR_VALUE * pow(t, 1.5));
	g = (int)(MAX_COLOR_VALUE * pow(t, 3.0));
	b = (int)(MAX_COLOR_VALUE * pow(t, 4.5));
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	r = MIN(MAX(r, 0), MAX_COLOR_VALUE);
	g = MIN(MAX(g, 0), MAX_COLOR_VALUE);
	b = MIN(MAX(b, 0), MAX_COLOR_VALUE);
	return ((r << 16) | (g << 8) | b);
}

int	get_color_stripes(int iter, t_data *data)
{
	double	t;
	double	value;
	int		color;

	t = (double)iter / data->max_iter;
	value = 0.5 * sin(10.0 * t * M_PI) + 0.5;
	color = (int)(value * MAX_COLOR_VALUE);
	return ((color << 16) | (color << 8) | color);
}

int	get_color_smooth(int iter, t_data *data)
{
	double	t;
	int		r;
	int		g;
	int		b;

	t = (double)iter / data->max_iter;
	r = (int)(9 * (1 - t) * t * t * t * MAX_COLOR_VALUE);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * MAX_COLOR_VALUE);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * MAX_COLOR_VALUE);
	return ((r << 16) | (g << 8) | b);
}

int	get_color_classic(int iter, t_data *data)
{
	int	color;

	color = (iter * MAX_COLOR_VALUE / data->max_iter) % (MAX_COLOR_VALUE + 1);
	return ((color << 16) | (color << 8) | color);
}

int	get_color_logarithmic(int iter, int max_iter, t_data *data)
{
	double	normalized;
	int		color;

	(void)data;
	if (iter < max_iter)
		normalized = log((double)iter) / log((double)max_iter);
	else
		normalized = 0;
	color = (int)(normalized * MAX_COLOR_VALUE);
	return ((color << 16) | (color << 8) | color);
}

int	get_color_hsv(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	int			r;
	int			g;
	int			b;

	vars.t = (double)iter / max_iter;
	vars.h = vars.t * HSV_HUE_MAX;
	hsv_to_rgb(vars.h, HSV_SATURATION, HSV_VALUE, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

void yuv_to_rgb(double y, double u, double v, int *r, int *g, int *b)
{
	double r_f, g_f, b_f;

	r_f = y + 1.407 * v;
	g_f = y - 0.344 * u - 0.714 * v;
	b_f = y + 1.770 * u;

	r_f = fmin(fmax(r_f, 0.0), 1.0);
	g_f = fmin(fmax(g_f, 0.0), 1.0);
	b_f = fmin(fmax(b_f, 0.0), 1.0);

	*r = (int)(r_f * 255.0);
	*g = (int)(g_f * 255.0);
	*b = (int)(b_f * 255.0);
}



void	hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b)
{
	double	c;
	double	x;
	double	m;
	int		hi;

	c = v * s;
	h = fmod(h, HSV_HUE_MAX) / 60.0;
	x = c * (1 - fabs(fmod(h, 2) - 1));
	hi = (int)h;
	m = v - c;
	if (hi == 0)
		(*r = c + m, *g = x + m, *b = m);
	else if (hi == 1)
		(*r = x + m, *g = c + m, *b = m);
	else if (hi == 2)
		(*r = m, *g = c + m, *b = x + m);
	else if (hi == 3)
		(*r = m, *g = x + m, *b = c + m);
	else if (hi == 4)
		(*r = x + m, *g = m, *b = c + m);
	else
		(*r = c + m, *g = m, *b = x + m);
	*r = (int)(*r * MAX_COLOR_VALUE);
	*g = (int)(*g * MAX_COLOR_VALUE);
	*b = (int)(*b * MAX_COLOR_VALUE);
}

int	get_color_gradient(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	int			r;
	int			g;
	int			b;

	vars.t = (double)iter / max_iter;
	r = (int)((1 - vars.t) * 0 + vars.t * MAX_COLOR_VALUE);
	g = (int)((1 - vars.t) * 0 + vars.t * 165);
	b = (int)((1 - vars.t) * MAX_COLOR_VALUE + vars.t * 0);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

static void	calculate_magnitude(double z_real, double z_imag,
		double *magnitude_sq, double *magnitude_val)
{
	*magnitude_sq = z_real * z_real + z_imag * z_imag;
	*magnitude_val = sqrt(*magnitude_sq);
}

static double	calculate_angle(double z_real, double z_imag)
{
	double	angle;

	angle = atan2(z_imag, z_real);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

static void	calculate_uv_components(double angle, double saturation,
		double *u, double *v)
{
	*u = cos(angle) * saturation;
	*v = sin(angle) * saturation;
}

static int	apply_base_color(t_data *data, int r, int g, int b)
{
	r = (int)((double)r * data->base_color.r / MAX_COLOR_VALUE);
	g = (int)((double)g * data->base_color.g / MAX_COLOR_VALUE);
	b = (int)((double)b * data->base_color.b / MAX_COLOR_VALUE);
	return ((r << 16) | (g << 8) | b);
}

int	get_color_custom_interior(int iter, int max_iter,
		t_data *data, double z_real, double z_imag)
{
	double	magnitude_sq;
	double	magnitude_val;
	double	escape_value;
	double	angle;
	int		r;
	int		g;
	int		b;

	if (iter >= max_iter)
		return (COLOR_BLACK);
	calculate_magnitude(z_real, z_imag, &magnitude_sq, &magnitude_val);
	escape_value = log(magnitude_sq) * magnitude_val / 65536.0;
	angle = calculate_angle(z_real, z_imag);
	double u;
	double v;
	calculate_uv_components(angle, 1.0, &u, &v);
	yuv_to_rgb(fmax(0.0, 1.0 - escape_value / 4.0), u, v, &r, &g, &b);
	return (apply_base_color(data, r, g, b));
}



int	get_color_black_white(int iter, t_data *data)
{
	if (iter % 2 == 0)
		return ((data->base_color.r << 16) | (data->base_color.g << 8)
			| data->base_color.b);
	return (COLOR_BLACK);
}

int	get_color_escape_time(int iter, int max_iter, t_data *data)
{
	double	t;
	int		color;

	t = (double)iter / max_iter;
	color = (int)(t * MAX_COLOR_VALUE);
	color = (color * data->base_color.r) / MAX_COLOR_VALUE;
	return ((color << 16) | (color << 8) | color);
}

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
	r = (int)(MAX_COLOR_VALUE * t * (data->base_color.r / 255.0));
	g = (int)(MAX_COLOR_VALUE * (1 - t) * (data->base_color.g / 255.0));
	b = (int)(MAX_COLOR_VALUE * (t * (1 - t)) * (data->base_color.b / 255.0));
	return ((r << 16) | (g << 8) | b);
}

int	get_color_derivative_bailout(int iter, double z_real,
		double z_imag, t_data *data)
{
	double	magnitude;
	int		color;

	(void)data;
	(void)iter;
	magnitude = sqrt(z_real * z_real + z_imag * z_imag);
	color = (int)(MAX_COLOR_VALUE * (1 - exp(-magnitude)));
	return ((color << 16) | (color << 8) | color);
}

int	get_color_interior_distance(int iter, double z_real,
		double z_imag, int max_iter, t_data *data)
{
	double	distance;
	double	normalized;
	int		color;

	(void)data;
	if (iter == max_iter)
	{
		distance = sqrt(z_real * z_real + z_imag * z_imag);
		normalized = log(distance) / log(ESCAPE_RADIUS);
		color = (int)(normalized * MAX_COLOR_VALUE) % (MAX_COLOR_VALUE + 1);
		return ((color << 16) | (color << 8) | color);
	}
	return (COLOR_BLACK);
}

int	get_color_lch(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int				b;

	vars.t = (double)iter / max_iter;
	lch.l = LCH_L_BASE + LCH_L_RANGE * sin(M_PI * vars.t);
	lch.c = LCH_C_BASE;
	lch.h_deg = HSV_HUE_MAX * vars.t;
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	if (x >= 0 && x < data->width && y >= 0 && y < data->height)
	{
		int	index = y * data->line_len + x * (data->bpp / 8);
		*((unsigned int *)(data->addr + index)) = color;
	}
}


char	*str_join_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

void	draw_info_strings(t_data *data)
{
	int		y;

	if (!data->overlay_enabled)
		return ;
	y = 10;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, "Fract'ol - 42 Project");
	y += 20;
	draw_controls(data, &y);
	draw_parameters(data, &y);
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, data->fast_mode ? "Mode Rapide: Activé" : "Mode Rapide: Désactivé");
	y += 20;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, data->is_fullscreen ? "Plein Écran: Activé" : "Plein Écran: Désactivé");
	y += 20;
}


void	draw_controls(t_data *data, int *y)
{
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Press R to reset view");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Use arrow keys or WASD to move");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Use + and - to zoom");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Adjust iterations with F (down) and G (up)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Change palette with Z (previous) and X (next)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Fast Mode with 1");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Overlay with H");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Fullscreen with 0");
	*y += 20;
}


static void	draw_zoom_info(t_data *data, int *y)
{
	char	*str;
	char	*temp;

	temp = ft_ftoa(data->zoom, 2);
	str = ft_strjoin("Zoom: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;
}

static void	draw_iter_info(t_data *data, int *y)
{
	char	*str;
	char	*temp;

	temp = ft_itoa(data->max_iter);
	str = ft_strjoin("Iterations: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;
}

static char	*create_rgb_string(t_data *data)
{
	char	*str;

	str = ft_strjoin("RGB: (", ft_itoa(data->base_color.r));
	if (!str)
		return (NULL);
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.g));
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.b));
	str = str_join_free(str, ft_strdup(")"));
	return (str);
}

static void	draw_rgb_info(t_data *data, int *y)
{
	char	*str;

	str = create_rgb_string(data);
	if (str)
	{
		mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
		free(str);
	}
	*y += 20;
}

void	draw_parameters(t_data *data, int *y)
{
	draw_zoom_info(data, y);
	draw_iter_info(data, y);
	draw_rgb_info(data, y);
	draw_fractal_type(data, y);
	draw_palette_type(data, y);
}
void	draw_fractal_type(t_data *data, int *y)
{
	const char	*fractal_names[] = {
		"Julia", "Mandelbrot", "Tricorn", "Burning Ship",
		"Lyapunov", "Newton", "Buddhabrot"
	};

	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Fractal Type: ");
	mlx_string_put(data->mlx, data->win, 120, *y, COLOR_WHITE,
		(char *)fractal_names[data->fractal_type]);
	*y += 20;
}

void	draw_palette_type(t_data *data, int *y)
{
	const char	*palette_names[] = {
		"Fire",
		"Stripes",
		"Smooth",
		"Classic",
		"Logarithmic",
		"HSV",
		"Gradient",
		"Black & White",
		"Escape Time",
		"Continuous Potential",
		"Interior Distance",
		"LCH",
		"Exp Cyclic LCH No Shading",
		"Exp Cyclic LCH Shading",
		"Derivative Bailout",
		"Dwell Gradient",
		"Rainbow Interior"
	};

	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Palette Type: ");
	mlx_string_put(data->mlx, data->win, 120, *y, COLOR_WHITE,
		(char *)palette_names[data->palette_type]);
	*y += 20;
}

void	draw_selection_rectangle(t_data *data)
{
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return;
	draw_rectangle_edges(data, x_start, y_start, x_end, y_end);
}

void	get_sorted_selection(t_data *data, int *x_start, int *x_end, int *y_start, int *y_end)
{
	*x_start = ft_clamp((data->select_start_x < data->select_end_x) ? data->select_start_x : data->select_end_x, 0, data->width - 1);
	*x_end = ft_clamp((data->select_start_x > data->select_end_x) ? data->select_start_x : data->select_end_x, 0, data->width - 1);
	*y_start = ft_clamp((data->select_start_y < data->select_end_y) ? data->select_start_y : data->select_end_y, 0, data->height - 1);
	*y_end = ft_clamp((data->select_start_y > data->select_end_y) ? data->select_start_y : data->select_end_y, 0, data->height - 1);
}

void	draw_rectangle_edges(t_data *data, int x_start, int y_start, int x_end, int y_end)
{
	int	x;

	for (x = x_start; x <= x_end; x++)
	{
		put_pixel(data, x, y_start, COLOR_WHITE);
		put_pixel(data, x, y_end, COLOR_WHITE);
	}

	int	y;
	for (y = y_start; y <= y_end; y++)
	{
		put_pixel(data, x_start, y, COLOR_WHITE);
		put_pixel(data, x_end, y, COLOR_WHITE);
	}
}

void	swap_int(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	calculate_buddha_bounds(t_data *data, int x_start,
		int x_end, int y_start, int y_end)
{
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	x_range;
	double	y_range;

	x_range = data->buddha_real_max - data->buddha_real_min;
	y_range = data->buddha_imag_max - data->buddha_imag_min;
	x_min = data->buddha_real_min + (double)x_start / WIN_WIDTH * x_range;
	x_max = data->buddha_real_min + (double)x_end / WIN_WIDTH * x_range;
	y_min = data->buddha_imag_min + (double)y_start / WIN_HEIGHT * y_range;
	y_max = data->buddha_imag_min + (double)y_end / WIN_HEIGHT * y_range;
	data->buddha_real_min = x_min;
	data->buddha_real_max = x_max;
	data->buddha_imag_min = y_min;
	data->buddha_imag_max = y_max;
}

static void	calculate_coordinates(t_data *data, int x, int y,
		double *coord_x, double *coord_y)
{
	*coord_x = (x / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom
		+ data->offset_x;
	*coord_y = (y / (double)WIN_HEIGHT - 0.5) * 4.0 / data->zoom
		+ data->offset_y;
}

static void	update_zoom_and_center(t_data *data, double start_x,
		double end_x, double start_y, double end_y)
{
	double	selected_width;
	double	selected_height;
	double	zoom_factor_x;
	double	zoom_factor_y;
	double	new_center_x;
	double	new_center_y;

	selected_width = fabs(end_x - start_x);
	selected_height = fabs(end_y - start_y);
	zoom_factor_x = 4.0 / (selected_width * data->zoom);
	zoom_factor_y = 4.0 / (selected_height * data->zoom);
	data->zoom *= fmin(zoom_factor_x, zoom_factor_y);
	new_center_x = start_x + selected_width / 2.0;
	new_center_y = start_y + selected_height / 2.0;
	data->offset_x = new_center_x;
	data->offset_y = new_center_y;
}

void	zoom_to_selection(t_data *data)
{
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;
	double	start_x;
	double	start_y;
	double	end_x;
	double	end_y;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return ;
	if (data->fractal_type == BUDDHABROT)
		calculate_buddha_bounds(data, x_start, x_end, y_start, y_end);
	else
	{
		calculate_coordinates(data, x_start, y_start, &start_x, &start_y);
		calculate_coordinates(data, x_end, y_end, &end_x, &end_y);
		update_zoom_and_center(data, start_x, end_x, start_y, end_y);
	}
	data->redraw = 1;
}


void calculate_zoom_and_offset(t_data *data, int x_start, int x_end, int y_start, int y_end)
{
	double x_min;
	double x_max;
	double y_min;
	double y_max;	

	x_min = data->offset_x + (x_start - WIN_WIDTH / 2.0) * (4.0 / (WIN_WIDTH * data->zoom));
	x_max = data->offset_x + (x_end - WIN_WIDTH / 2.0) * (4.0 / (WIN_WIDTH * data->zoom));
	y_min = data->offset_y + (y_start - WIN_HEIGHT / 2.0) * (4.0 / (WIN_HEIGHT * data->zoom));
	y_max = data->offset_y + (y_end - WIN_HEIGHT / 2.0) * (4.0 / (WIN_HEIGHT * data->zoom));	
	update_zoom_and_offset(data, x_min, x_max, y_min, y_max);
}

void update_zoom_and_offset(t_data *data, double x_min, double x_max, double y_min, double y_max)
{
	double new_width = x_max - x_min;
	double new_height = y_max - y_min;
	double zoom_ratio_x;
	double zoom_ratio_y;
	double new_zoom;

	zoom_ratio_x = WIN_WIDTH / new_width;
	zoom_ratio_y = WIN_HEIGHT / new_height;
	new_zoom = fmin(zoom_ratio_x, zoom_ratio_y);
	data->zoom *= new_zoom;
	data->offset_x = (x_min + x_max) / 2.0;
	data->offset_y = (y_min + y_max) / 2.0;
}

static int	handle_allocation(char **int_str, char **frac_str,
		long long int_part, int precision)
{
	*int_str = ft_lltoa(int_part);
	if (!(*int_str))
		return (0);
	*frac_str = (char *)malloc(precision + 2);
	if (!(*frac_str))
	{
		free(*int_str);
		return (0);
	}
	return (1);
}

static void	process_fractional_part(char *frac_str, double frac_part,
		int precision)
{
	int	i;

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
}

static char	*join_and_free(char *int_str, char *frac_str)
{
	char	*result;

	result = ft_strjoin(int_str, frac_str);
	free(int_str);
	free(frac_str);
	return (result);
}

char	*ft_ftoa(double n, int precision)
{
	long long	int_part;
	double		frac_part;
	char		*int_str;
	char		*frac_str;

	int_part = (long long)n;
	frac_part = n - (double)int_part;
	if (frac_part < 0)
		frac_part = -frac_part;
	if (!handle_allocation(&int_str, &frac_str, int_part, precision))
		return (NULL);
	process_fractional_part(frac_str, frac_part, precision);
	return (join_and_free(int_str, frac_str));
}

char	*ft_lltoa(long long n)
{
	char		*str;
	int			len;
	int			sign;
	long long	nb;

	sign = (n < 0);
	nb = (n < 0) ? -n : n;
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

int	ft_numlen(long long n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

int	compute_fractal(t_data *data, t_fractal_vars *vars, int iter_count)
{
	if (data->fractal_type == JULIA)
		return (compute_julia(data, vars, iter_count));
	else if (data->fractal_type == MANDELBROT
		|| data->fractal_type == TRICORN)
		return (compute_mandelbrot(data, vars, iter_count));
	else if (data->fractal_type == BURNING_SHIP)
		return (compute_burning_ship(vars, iter_count));
	else if (data->fractal_type == LYAPUNOV)
		return (compute_lyapunov(vars, iter_count));
	else if (data->fractal_type == NEWTON)
		return (compute_newton(vars, iter_count));
	return (0);
}

int	get_color_exp_cyclic_lch_no_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int			b;

	vars.t = exp((double)-iter / max_iter);
	lch.l = LCH_L_BASE;
	lch.c = LCH_C_BASE;
	lch.h_deg = fmod(HSV_HUE_MAX * vars.t * LCH_H_MULTIPLIER, HSV_HUE_MAX);
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_exp_cyclic_lch_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int				b;

	vars.t = exp((double)-iter / max_iter);
	lch.l = LCH_L_BASE + LCH_L_RANGE * sin(2 * M_PI * vars.t);
	lch.c = LCH_C_BASE + LCH_L_RANGE * sin(2 * M_PI * vars.t);
	lch.h_deg = fmod(HSV_HUE_MAX * vars.t * LCH_H_MULTIPLIER, HSV_HUE_MAX);
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

void	lch_to_rgb(t_lch_color *lch, int *r, int *g, int *b)
{
	t_color_vars	vars;

	vars.h = lch->h_deg * M_PI / 180.0;
	*r = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 + lch->c * cos(vars.h) / 100.0));
	*g = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0 
		+ 0.866 * lch->c * sin(vars.h) / 100.0));
	*b = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0 
		- 0.866 * lch->c * sin(vars.h) / 100.0));
	*r = MIN(MAX(*r, 0), MAX_COLOR_VALUE);
	*g = MIN(MAX(*g, 0), MAX_COLOR_VALUE);
	*b = MIN(MAX(*b, 0), MAX_COLOR_VALUE);
}

int	compute_julia(t_data *data, t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
	iter = 0;
	while (((z_real_sq = vars->z_real * vars->z_real) + (z_imag_sq = vars->z_imag * vars->z_imag)) <= ESCAPE_RADIUS
		&& (iter < iter_count))
	{
		tmp = z_real_sq - z_imag_sq + data->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + data->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

static void	init_mandel_vars(t_fractal_vars *vars)
{
	vars->z_real = 0.0;
	vars->z_imag = 0.0;
}

static int	compute_tricorn(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	iter = 0;
	while (iter < iter_count)
	{
		z_real_sq = vars->z_real * vars->z_real;
		z_imag_sq = vars->z_imag * vars->z_imag;
		if (z_real_sq + z_imag_sq > ESCAPE_RADIUS)
			break ;
		tmp = z_real_sq - z_imag_sq + vars->c_real;
		vars->z_imag = -2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

static int	compute_standard(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	iter = 0;
	while (iter < iter_count)
	{
		z_real_sq = vars->z_real * vars->z_real;
		z_imag_sq = vars->z_imag * vars->z_imag;
		if (z_real_sq + z_imag_sq > ESCAPE_RADIUS)
			break ;
		tmp = z_real_sq - z_imag_sq + vars->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

int	compute_mandelbrot(t_data *data, t_fractal_vars *vars, int iter_count)
{
	init_mandel_vars(vars);
	if (data->fractal_type == TRICORN)
		return (compute_tricorn(vars, iter_count));
	return (compute_standard(vars, iter_count));
}

int	compute_burning_ship(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;

	vars->z_real = 0;
	vars->z_imag = 0;
	iter = 0;
	while ((vars->z_real * vars->z_real + vars->z_imag * vars->z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		vars->z_real = fabs(vars->z_real);
		vars->z_imag = fabs(vars->z_imag);
		tmp = vars->z_real * vars->z_real - vars->z_imag * vars->z_imag + vars->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

static void	init_lyap_vars(double *value, double *sum_log_deriv,
		int *max_iter, int iter_count)
{
	*value = 0.5;
	*sum_log_deriv = 0.0;
	*max_iter = iter_count * ft_strlen(LYAPUNOV_SEQUENCE);
}

static double	get_r_value(t_fractal_vars *vars, char sequence_char)
{
	if (sequence_char == 'A')
		return (vars->c_real);
	return (vars->c_imag);
}

static int	check_value_bounds(double value, double r)
{
	if (r < 0 || r > 4)
		return (0);
	if (value == 0 || value == 1)
		return (0);
	return (1);
}

static int	calculate_final_value(double sum_log_deriv, int iter)
{
	if (iter == 0)
		return (0);
	sum_log_deriv /= iter;
	if (sum_log_deriv > 0)
		return ((int)(sum_log_deriv * MAX_COLOR_VALUE));
	return (0);
}

int	compute_lyapunov(t_fractal_vars *vars, int iter_count)
{
	double			value;
	double			sum_log_deriv;
	int				iter;
	int				max_iter;
	double			r;
	unsigned int	m;
	char			*sequence;

	sequence = LYAPUNOV_SEQUENCE;
	init_lyap_vars(&value, &sum_log_deriv, &max_iter, iter_count);
	iter = 0;
	while (iter < max_iter)
	{
		m = iter % ft_strlen(sequence);
		r = get_r_value(vars, sequence[m]);
		if (!check_value_bounds(value, r))
			break ;
		value = r * value * (1 - value);
		sum_log_deriv += log(fabs(r * (1 - 2 * value)));
		iter++;
	}
	return (calculate_final_value(sum_log_deriv, iter));
}

static void	init_newton_vars(t_fractal_vars *vars)
{
	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
}

static double	calculate_denominator(double old_real, double old_imag)
{
	double	diff_squared;

	diff_squared = old_real * old_real - old_imag * old_imag;
	return (3.0 * diff_squared * diff_squared);
}

static void	calculate_next_z(t_fractal_vars *vars, double old_real,
		double old_imag, double denominator)
{
	double	tmp;
	double	real_cubed;
	double	imag_cubed;

	real_cubed = old_real * old_real * old_real;
	imag_cubed = old_imag * old_imag * old_imag;
	tmp = (2.0 * real_cubed - 2.0 * old_real * old_imag * old_imag
			- old_real) / denominator;
	vars->z_imag = (2.0 * imag_cubed - 2.0 * old_real * old_real
			* old_imag + old_imag) / denominator;
	vars->z_real = tmp;
}

static int	check_convergence(double old_real, double old_imag,
		double new_real, double new_imag)
{
	double	diff_real;
	double	diff_imag;

	diff_real = old_real - new_real;
	diff_imag = old_imag - new_imag;
	return ((diff_real * diff_real + diff_imag * diff_imag) < 1e-6);
}

int	compute_newton(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	old_real;
	double	old_imag;
	double	denominator;

	init_newton_vars(vars);
	iter = 0;
	while (iter < iter_count)
	{
		old_real = vars->z_real;
		old_imag = vars->z_imag;
		denominator = calculate_denominator(old_real, old_imag);
		if (denominator == 0)
			break ;
		calculate_next_z(vars, old_real, old_imag, denominator);
		if (check_convergence(old_real, old_imag,
				vars->z_real, vars->z_imag))
			break ;
		iter++;
	}
	return (iter);
}

void	ft_swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	ft_clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}