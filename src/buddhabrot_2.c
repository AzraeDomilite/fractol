/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddhabrot_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:27:13 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 16:27:37 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

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
	t_thread_data	*thread;
	int				i;
	double			c_real;
	double			c_imag;
	int				samples_per_thread;
	t_data			*data;

	thread = NULL;
	data = thread->data;
	thread = (t_thread_data *)arg;
	samples_per_thread = SAMPLES_PER_THREAD;
	srand(thread->seed);
	for (i = 0; i < samples_per_thread; i++)
	{
		c_real = data->buddha_real_min + ((double)rand() / RAND_MAX) * (data->buddha_real_max - data->buddha_real_min);
		c_imag = data->buddha_imag_min + ((double)rand() / RAND_MAX) * (data->buddha_imag_max - data->buddha_imag_min);
		process_point(data, c_real, c_imag);
	}
	return (NULL);
}
