/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:00:32 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:00:44 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

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
