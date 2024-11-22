/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_rgb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:10:01 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 19:10:01 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

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

int	init_main_histogram(t_data *data)
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

int	init_thread_resources(t_thread_data **thread_data,
		pthread_t **threads)
{
	*threads = (pthread_t *)malloc(NUM_THREADS * sizeof(pthread_t));
	*thread_data = (t_thread_data *)malloc(NUM_THREADS * sizeof(t_thread_data));
	if (!(*threads) || !(*thread_data))
		return (0);
	return (1);
}

int	init_thread_data_arrays(t_thread_data *thread_data,
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

int	create_and_run_threads(t_thread_data *thread_data,
		pthread_t *threads, t_data *data)
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
