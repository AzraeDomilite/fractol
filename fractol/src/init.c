/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:39:18 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:42:42 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

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
}

void	init_data_colors(t_data *data)
{
	data->palette_type = PALETTE_GRADIENT;
	data->base_color.r = MAX_COLOR_VALUE;
	data->base_color.g = MAX_COLOR_VALUE;
	data->base_color.b = MAX_COLOR_VALUE;
	data->line_brightness = 0.0;
	data->cell_brightness = 0.75;
	data->base_saturation = 0.25;
}

void	init_data_params(t_data *data)
{
	data->branch_factor = 2.0;
	data->branch_offset = 1.0;
	data->branch_base = 3.0;
	data->line_width_base = 0.25;
	data->overlay_enabled = 1;
	data->fast_mode = 0;
	data->is_fullscreen = 0;
}

void	init_data_dimensions(t_data *data)
{
	data->original_width = WIN_WIDTH;
	data->original_height = WIN_HEIGHT;
	data->width = WIN_WIDTH;
	data->height = WIN_HEIGHT;
}

void	init_fractal_params(t_data *data, int iter_count)
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
