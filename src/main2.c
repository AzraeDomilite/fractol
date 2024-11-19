/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 05:20:50 by blucken           #+#    #+#             */
/*   Updated: 2024/11/19 05:24:06 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

pthread_mutex_t	g_histogram_mutex = PTHREAD_MUTEX_INITIALIZER;

void	init_data(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
	data->zoom = 1.0;
	data->c_real = INITIAL_C_REAL;
	data->c_imag = INITIAL_C_IMAG;
	data->redraw = 1;
	data->max_iter = DEFAULT_PREVIEW_ITER;
	data->fractal_type = JULIA;
	data->palette_type = PALETTE_FIRE;
	data->base_color.r = MAX_COLOR_VALUE;
	data->base_color.g = MAX_COLOR_VALUE;
	data->base_color.b = MAX_COLOR_VALUE;
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

int	deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);
	data->fast_mode = 1;
	return (0);
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
		move_offset(data, -MOVE_FACTOR, 0);
}

void	move_offset(t_data *data, double x_factor, double y_factor)
{
	data->offset_x += x_factor / data->zoom;
	data->offset_y += y_factor / data->zoom;
	data->redraw = 1;
}

void	handle_zoom(int key, t_data *data)
{
	double	center_re;
	double	center_im;

	if (key == K_NUM_PLUS || key == K_NUM_MINUS)
	{
		center_re = data->offset_x;
		center_im = data->offset_y;

		if (key == K_NUM_PLUS)
			data->zoom *= ZOOM_FACTOR;
		else
			data->zoom /= ZOOM_FACTOR;

		data->redraw = 1;
	}
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
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	draw_fractal(data, iter_count);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	draw_info_strings(data);
	if (data->is_selecting)
		draw_selection_rectangle(data);
}

void	draw_fractal(t_data *data, int iter_count)
{
	pthread_t		threads[NUM_THREADS];
	t_data			thread_data[NUM_THREADS];
	int				i;
	int				height_per_thread;

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

	height_per_thread = WIN_HEIGHT / NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i] = *data;
		thread_data[i].y_start = i * height_per_thread;
		thread_data[i].y_end = (i + 1) * height_per_thread;
		if (i == NUM_THREADS - 1)
			thread_data[i].y_end = WIN_HEIGHT;
		if (pthread_create(&threads[i], NULL, thread_draw_fractal, &thread_data[i]) != 0)
		{
			ft_printf(ERROR_THREAD_CREATE);
			while (--i >= 0)
				pthread_join(threads[i], NULL);
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

void	render_buddhabrot(t_data *data)
{
	t_thread_data	thread_data[NUM_THREADS];
	pthread_t		threads[NUM_THREADS];
	int				i;
	int				j;

	data->histogram = ft_calloc(WIN_WIDTH * WIN_HEIGHT, sizeof(unsigned int));
	if (!data->histogram)
	{
		ft_printf(ERROR_MALLOC_HIST);
		exit_fractol(data);
	}
	data->scale = 4.0 / (WIN_WIDTH * data->zoom);
	data->real_min = -2.5 / data->zoom + data->offset_x;
	data->imag_min = -1.5 / data->zoom + data->offset_y;
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].data = data;
		thread_data[i].seed = i + 1;
		thread_data[i].samples = SAMPLES_PER_THREAD;
		if (pthread_create(&threads[i], NULL, thread_generate_buddhabrot,
				&thread_data[i]) != 0)
		{
			j = 0;
			while (j < i)
				pthread_join(threads[j++], NULL);
			free(data->histogram);
			ft_printf(ERROR_THREAD_CREATE);
			exit_fractol(data);
		}
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	normalize_and_render_buddhabrot(data);
	free(data->histogram);
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

void	process_buddhabrot_point(t_data *data, double c_real, double c_imag)
{
	double	z_real;
	double	z_imag;
	double	trajectory_real[MAX_ITER];
	double	trajectory_imag[MAX_ITER];
	double	tmp;
	int		iter;

	// Vérifier si le point est dans le cardioid principal ou le bulbe période-2
	if (is_in_main_cardioid(c_real, c_imag) || is_in_period2_bulb(c_real, c_imag))
		return ;

	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	
	while (iter < data->max_iter)
	{
		// Sauvegarder la trajectoire
		trajectory_real[iter] = z_real;
		trajectory_imag[iter] = z_imag;
		
		// Calcul de Mandelbrot standard
		tmp = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp;
		
		if (z_real * z_real + z_imag * z_imag > 4.0)
		{
			// Le point s'échappe, on trace sa trajectoire
			update_histogram(data, trajectory_real, trajectory_imag, iter);
			break ;
		}
		iter++;
	}
}

void	update_histogram(t_data *data, double *traj_real,
			double *traj_imag, int length)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	while (i < length)
	{
		// Convertir les coordonnées complexes en coordonnées d'écran
		x = (int)((traj_real[i] - data->real_min) / data->scale);
		y = (int)((traj_imag[i] - data->imag_min) / data->scale);
		
		if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		{
			pthread_mutex_lock(&data->histogram_mutex);
			data->histogram[y * WIN_WIDTH + x]++;
			pthread_mutex_unlock(&data->histogram_mutex);
		}
		i++;
	}
}

void	normalize_and_render_buddhabrot(t_data *data)
{
	unsigned int	max_value;
	unsigned int	value;
	double		normalized;
	int			color;
	int			i;
	int			x;
	int			y;

	// Trouver la valeur maximale dans l'histogramme
	max_value = 0;
	i = 0;
	while (i < WIN_WIDTH * WIN_HEIGHT)
	{
		if (data->histogram[i] > max_value)
			max_value = data->histogram[i];
		i++;
	}
	
	if (max_value == 0)
		return ;

	// Normaliser et afficher
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			value = data->histogram[y * WIN_WIDTH + x];
			// Utiliser une échelle logarithmique pour mieux distribuer les valeurs
			normalized = log(1 + value) / log(1 + max_value);
			color = (int)(normalized * 255);
			
			// Créer une couleur en niveaux de gris
			put_pixel(data, x, y, (color << 16) | (color << 8) | color);
			x++;
		}
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
	return (squared <= 0.0625);  // 1/16
}

int	get_color(int iter, t_data *data, double z_real, double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_FIRE)
		return (get_color_fire(iter, max_iter, data));
	else if (data->palette_type == PALETTE_STRIPES)
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
	else if (data->palette_type == PALETTE_INTERIOR_DISTANCE)
		return (get_color_interior_distance(iter, z_real, z_imag,
				max_iter, data));
	else if (data->palette_type == PALETTE_LCH)
		return (get_color_lch(iter, max_iter, data));
	else if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_NO_SHADING)
		return (get_color_exp_cyclic_lch_no_shading(iter, max_iter, data));
	else if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_SHADING)
		return (get_color_exp_cyclic_lch_shading(iter, max_iter, data));
	return (COLOR_BLACK);
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
	t_lch_color	lch;
	int			r;
	int			g;
	int			b;

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
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
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

	y = 10;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE,
		"Fract'ol - 42 Project");
	y += 20;
	draw_controls(data, &y);
	draw_parameters(data, &y);
}

void	draw_controls(t_data *data, int *y)
{
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Press R to reset view");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Use arrow keys or WASD to move");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Use + and - to zoom");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Adjust iterations with F (down) and G (up)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Change palette with Z (previous) and X (next)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Adjust color components with Numpad 7-9, 4-6, 1-3");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Reset color components with Numpad 8, 5, 2");
	*y += 20;
}

void	draw_parameters(t_data *data, int *y)
{
	char	*str;
	char	*temp;

	temp = ft_ftoa(data->zoom, 2);
	str = ft_strjoin("Zoom: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;

	temp = ft_itoa(data->max_iter);
	str = ft_strjoin("Iterations: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;

	str = ft_strjoin("RGB: (", ft_itoa(data->base_color.r));
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.g));
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.b));
	str = str_join_free(str, ft_strdup(")"));
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;

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
		"Fire", "Stripes", "Smooth", "Classic", "Logarithmic",
		"HSV", "Gradient", "Black & White", "Escape Time",
		"Continuous Potential", "Interior Distance", "LCH",
		"Exp Cyclic LCH No Shading", "Exp Cyclic LCH Shading",
		"Derivative Bailout"
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
	draw_rectangle_edges(data, x_start, y_start, x_end, y_end);
}

void	get_sorted_selection(t_data *data, int *x_start, int *x_end,
	int *y_start, int *y_end)
{
	*x_start = data->select_start_x;
	*y_start = data->select_start_y;
	*x_end = data->select_end_x;
	*y_end = data->select_end_y;

	// S'assurer que les coordonnées sont dans le bon ordre
	if (*x_start > *x_end)
		ft_swap(x_start, x_end);
	if (*y_start > *y_end)
		ft_swap(y_start, y_end);

	// Limiter la sélection à la fenêtre
	*x_start = ft_clamp(*x_start, 0, WIN_WIDTH);
	*x_end = ft_clamp(*x_end, 0, WIN_WIDTH);
	*y_start = ft_clamp(*y_start, 0, WIN_HEIGHT);
	*y_end = ft_clamp(*y_end, 0, WIN_HEIGHT);
}

void	draw_rectangle_edges(t_data *data, int x_start, int y_start,
	int x_end, int y_end)
{
	int	x;
	int	y;

	// Dessiner les lignes horizontales
	y = y_start;
	while (y <= y_end)
	{
		if (y == y_start || y == y_end)
		{
			x = x_start;
			while (x <= x_end)
			{
				put_pixel(data, x, y, 0xFFFFFF);
				x++;
			}
		}
		else
		{
			put_pixel(data, x_start, y, 0xFFFFFF);
			put_pixel(data, x_end, y, 0xFFFFFF);
		}
		y++;
	}
}

void	swap_int(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	zoom_to_selection(t_data *data)
{
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;
	double	selected_width;
	double	selected_height;
	double	zoom_factor_x;
	double	zoom_factor_y;
	double	new_center_x;
	double	new_center_y;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return ;

	// Convertir le rectangle de sélection en coordonnées fractales
	double start_x = (x_start / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom + data->offset_x;
	double end_x = (x_end / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom + data->offset_x;
	double start_y = (y_start / (double)WIN_HEIGHT - 0.5) * 4.0 / data->zoom + data->offset_y;
	double end_y = (y_end / (double)WIN_HEIGHT - 0.5) * 4.0 / data->zoom + data->offset_y;

	// Calculer la largeur et hauteur de la sélection en coordonnées fractales
	selected_width = fabs(end_x - start_x);
	selected_height = fabs(end_y - start_y);

	// Calculer le facteur de zoom nécessaire pour que la sélection remplisse la fenêtre
	zoom_factor_x = 4.0 / (selected_width * data->zoom);
	zoom_factor_y = 4.0 / (selected_height * data->zoom);

	// Utiliser le facteur le plus petit pour maintenir le ratio
	data->zoom *= fmin(zoom_factor_x, zoom_factor_y);

	// Calculer le nouveau centre
	new_center_x = start_x + selected_width / 2.0;
	new_center_y = start_y + selected_height / 2.0;

	// Mettre à jour les offsets pour centrer la vue sur la sélection
	data->offset_x = new_center_x;
	data->offset_y = new_center_y;

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

    // Calculate new zoom ratios based on the selected area
    zoom_ratio_x = WIN_WIDTH / new_width;
    zoom_ratio_y = WIN_HEIGHT / new_height;

    // Choose the smaller zoom ratio to fit the selected area into the window
    new_zoom = fmin(zoom_ratio_x, zoom_ratio_y);

    // Update the zoom level
    data->zoom *= new_zoom;

    // Update offsets to center the selected area
    data->offset_x = (x_min + x_max) / 2.0;
    data->offset_y = (y_min + y_max) / 2.0;
}



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
	t_lch_color	lch;
	int		r;
	int		g;
	int		b;

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
	t_lch_color	lch;
	int		r;
	int		g;
	int		b;

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

	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
	iter = 0;
	while ((vars->z_real * vars->z_real + vars->z_imag * vars->z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		tmp = vars->z_real * vars->z_real - vars->z_imag * vars->z_imag + data->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + data->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

int	compute_mandelbrot(t_data *data, t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;

	vars->z_real = 0;
	vars->z_imag = 0;
	iter = 0;
	while ((vars->z_real * vars->z_real + vars->z_imag * vars->z_imag <= ESCAPE_RADIUS)
		&& (iter < iter_count))
	{
		tmp = vars->z_real * vars->z_real - vars->z_imag * vars->z_imag + vars->c_real;
		if (data->fractal_type == TRICORN)
			vars->z_imag = -2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		else
			vars->z_imag = 2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
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

int	compute_lyapunov(t_fractal_vars *vars, int iter_count)
{
	double			value;
	double			sum_log_deriv;
	int				iter;
	unsigned int	m;
	int				seq_len;
	char			*sequence;
	double			r;
	int				max_iter;

	sequence = LYAPUNOV_SEQUENCE;
	seq_len = ft_strlen(sequence);
	value = 0.5;
	sum_log_deriv = 0.0;
	iter = 0;
	max_iter = iter_count * seq_len;
	while (iter < max_iter)
	{
		m = iter % seq_len;
		if (sequence[m] == 'A')
			r = vars->c_real;
		else
			r = vars->c_imag;
		if (r < 0 || r > 4)
			break ;
		value = r * value * (1 - value);
		if (value == 0 || value == 1)
			break ;
		sum_log_deriv += log(fabs(r * (1 - 2 * value)));
		iter++;
	}
	if (iter == 0)
		return (0);
	sum_log_deriv /= iter;
	if (sum_log_deriv > 0)
		return ((int)(sum_log_deriv * MAX_COLOR_VALUE));
	return (0);
}

int	compute_newton(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	old_real;
	double	old_imag;
	double	denominator;
	double	tmp;

	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
	iter = 0;
	while (iter < iter_count)
	{
		old_real = vars->z_real;
		old_imag = vars->z_imag;
		denominator = 3 * (old_real * old_real - old_imag * old_imag)
			* (old_real * old_real - old_imag * old_imag);
		if (denominator == 0)
			break ;
		tmp = (2 * old_real * old_real * old_real
			- 2 * old_real * old_imag * old_imag - old_real)
			/ denominator;
		vars->z_imag = (2 * old_imag * old_imag * old_imag
			- 2 * old_real * old_real * old_imag + old_imag)
			/ denominator;
		vars->z_real = tmp;
		if ((old_real - vars->z_real) * (old_real - vars->z_real)
			+ (old_imag - vars->z_imag) * (old_imag - vars->z_imag) < 1e-6)
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