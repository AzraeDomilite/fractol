/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:42:05 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:42:34 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	move_offset(t_data *data, double x_factor, double y_factor)
{
	double	y_shift;
	double	x_shift;

	x_shift = x_factor * (4.0 / data->zoom);
	y_shift = y_factor * (3.0 / data->zoom);
	data->offset_x += x_shift;
	data->offset_y += y_shift;
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
