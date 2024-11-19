void	render_buddhabrot(t_data *data)
{
	t_thread_data	*thread_data;
	pthread_t		*threads;
	int				i;

	data->histogram = (unsigned int *)ft_calloc(WIN_WIDTH * WIN_HEIGHT,
			sizeof(unsigned int));
	if (!data->histogram)
	{
		ft_printf(ERROR_MALLOC_HIST);
		exit_fractol(data);
	}
	threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	thread_data = (t_thread_data *)malloc(NUM_THREADS * sizeof(t_thread_data));
	if (!threads || !thread_data)
	{
		free(data->histogram);
		free(threads);
		free(thread_data);
		ft_printf(ERROR_MALLOC_THREAD);
		exit_fractol(data);
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].data = data;
		thread_data[i].seed = i + 1;
		if (pthread_create(&threads[i], NULL, process_buddhabrot_section,
				&thread_data[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(threads[i], NULL);
			cleanup_buddhabrot(data, threads, thread_data);
			ft_printf(ERROR_THREAD_CREATE);
			exit_fractol(data);
		}
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	render_buddhabrot_image(data);
	cleanup_buddhabrot(data, threads, thread_data);
}

void	*process_buddhabrot_section(void *arg)
{
	t_thread_data	*thread;
	int				i;
	double			c_real;
	double			c_imag;
	int				samples_per_thread;

	thread = (t_thread_data *)arg;
	samples_per_thread = SAMPLES_PER_THREAD / NUM_THREADS;
	srand(thread->seed);
	i = 0;
	while (i < samples_per_thread)
	{
		c_real = BUDDHA_REAL_MIN + (rand() / (double)RAND_MAX)
			* (BUDDHA_REAL_MAX - BUDDHA_REAL_MIN);
		c_imag = BUDDHA_IMAG_MIN + (rand() / (double)RAND_MAX)
			* (BUDDHA_IMAG_MAX - BUDDHA_IMAG_MIN);
		process_point(thread->data, c_real, c_imag);
		i++;
	}
	return (NULL);
}

void	process_point(t_data *data, double c_real, double c_imag)
{
	double			z_real;
	double			z_imag;
	double			tmp;
	t_trajectory	traj;
	int				iter;

	if (is_in_main_cardioid(c_real, c_imag)
		|| is_in_period2_bulb(c_real, c_imag))
		return ;
	z_real = 0.0;
	z_imag = 0.0;
	iter = 0;
	while (iter < data->max_iter)
	{
		if (iter < MAX_ITER)
		{
			traj.real[iter] = z_real;
			traj.imag[iter] = z_imag;
		}
		tmp = z_real * z_real - z_imag * z_imag + c_real;
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = tmp;
		if (z_real * z_real + z_imag * z_imag > ESCAPE_RADIUS)
		{
			update_histogram(data, &traj, iter);
			break ;
		}
		iter++;
	}
}

void	update_histogram(t_data *data, t_trajectory *traj, int length)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	while (i < length && i < MAX_ITER)
	{
		x = (int)((traj->real[i] - BUDDHA_REAL_MIN)
				/ (BUDDHA_REAL_MAX - BUDDHA_REAL_MIN) * WIN_WIDTH);
		y = (int)((traj->imag[i] - BUDDHA_IMAG_MIN)
				/ (BUDDHA_IMAG_MAX - BUDDHA_IMAG_MIN) * WIN_HEIGHT);
		if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		{
			pthread_mutex_lock(&data->histogram_mutex);
			data->histogram[y * WIN_WIDTH + x]++;
			pthread_mutex_unlock(&data->histogram_mutex);
		}
		i++;
	}
}

void	render_buddhabrot_image(t_data *data)
{
	unsigned int	max_value;
	double		normalized;
	int			i;
	int			color;

	max_value = find_max_value(data->histogram, WIN_WIDTH * WIN_HEIGHT);
	if (max_value == 0)
		return ;
	i = 0;
	while (i < WIN_WIDTH * WIN_HEIGHT)
	{
		normalized = log(1 + data->histogram[i]) / log(1 + max_value);
		color = (int)(normalized * MAX_COLOR_VALUE);
		put_pixel(data, i % WIN_WIDTH, i / WIN_WIDTH,
			(color << 16) | (color << 8) | color);
		i++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
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

void	cleanup_buddhabrot(t_data *data, pthread_t *threads,
		t_thread_data *thread_data)
{
	free(data->histogram);
	free(threads);
	free(thread_data);
}