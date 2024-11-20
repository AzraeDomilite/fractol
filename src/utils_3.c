/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:38:13 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 19:38:13 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	update_histogram_point(t_data *data, int screen_x, int screen_y)
{
	if (screen_x >= 0 && screen_x < WIN_WIDTH
		&& screen_y >= 0 && screen_y < WIN_HEIGHT)
	{
		pthread_mutex_lock(&data->histogram_mutex);
		data->histogram[screen_y * WIN_WIDTH + screen_x]++;
		pthread_mutex_unlock(&data->histogram_mutex);
	}
}

void	calculate_screen_coords(double real, double imag,
		int *screen_x, int *screen_y)
{
	*screen_x = (int)((real - BUDDHA_REAL_MIN)
			/ (BUDDHA_REAL_MAX - BUDDHA_REAL_MIN) * WIN_WIDTH);
	*screen_y = (int)((imag - BUDDHA_IMAG_MIN)
			/ (BUDDHA_IMAG_MAX - BUDDHA_IMAG_MIN) * WIN_HEIGHT);
}

void	move_offset(t_data *data, double x_factor, double y_factor)
{
	double	y_shift;
	double	x_shift;

	if (data->fractal_type == BUDDHABROT)
	{
		x_shift = x_factor * (data->buddha_real_max - data->buddha_real_min);
		y_shift = y_factor * (data->buddha_imag_max - data->buddha_imag_min);
		data->buddha_real_min += x_shift;
		data->buddha_real_max += x_shift;
		data->buddha_imag_min += y_shift;
		data->buddha_imag_max += y_shift;
	}
	else
	{
		x_shift = x_factor * (4.0 / data->zoom);
		y_shift = y_factor * (3.0 / data->zoom);
		data->offset_x += x_shift;
		data->offset_y += y_shift;
	}
	data->redraw = 1;
}

void	calculate_buddha_bounds(t_data *data, t_zoom *zoom)
{
	t_bounds	bounds;

	bounds.x_range = data->buddha_real_max - data->buddha_real_min;
	bounds.y_range = data->buddha_imag_max - data->buddha_imag_min;
	bounds.x_min = data->buddha_real_min + (double)zoom->x_start
		/ WIN_WIDTH * bounds.x_range;
	bounds.x_max = data->buddha_real_min + (double)zoom->x_end
		/ WIN_WIDTH * bounds.x_range;
	bounds.y_min = data->buddha_imag_min + (double)zoom->y_start
		/ WIN_HEIGHT * bounds.y_range;
	bounds.y_max = data->buddha_imag_min + (double)zoom->y_end
		/ WIN_HEIGHT * bounds.y_range;
	data->buddha_real_min = bounds.x_min;
	data->buddha_real_max = bounds.x_max;
	data->buddha_imag_min = bounds.y_min;
	data->buddha_imag_max = bounds.y_max;
}
