/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:58:21 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:58:21 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

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

void	process_buddhabrot_point(t_data *data, double c_real, double c_imag)
{
	double	z_real;
	double	z_imag;
	double	tmp;
	double	trajectory_real[MAX_ITER];
	double	trajectory_imag[MAX_ITER];
	int		iter;

	if (is_in_main_cardioid(c_real, c_imag) || is_in_period2_bulb(c_real, c_imag))
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