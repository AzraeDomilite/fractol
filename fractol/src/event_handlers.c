/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:10:01 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 18:10:01 by blucken          ###   ########.ch       */
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
