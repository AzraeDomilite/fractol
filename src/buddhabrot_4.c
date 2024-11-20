/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddhabrot_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:23:59 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:25:15 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	allocate_trajectories(t_data *data, double **traj_real, double **traj_imag)
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

unsigned int	find_max_histogram_value(t_data *data)
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

void	render_line(t_data *data, int y, unsigned int max_value)
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

void	process_trajectory(t_data *data, double *traj_real, double *traj_imag, int iter)
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

int	calculate_color(unsigned int value, unsigned int max_value)
{
	double	normalized;
	int		color;

	normalized = log(1 + value) / log(1 + max_value);
	color = (int)(normalized * 255);
	return ((color << 16) | (color << 8) | color);
}
