/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:13:02 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:13:09 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	deal_key(int key, t_data *data)
{
	adjust_fractal_parameters(key, data);
	handle_reset_and_exit(key, data);
	handle_movement(key, data);
	handle_zoom(key, data);
	handle_iter_adjustment(key, data);
	if (key == K_1)
		handle_fast_mode(data);
	if (key == K_H)
		handle_overlay(data);
	if (key == K_0)
		handle_screen_mode(data);
	return (0);
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
