/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:15:22 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:15:22 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	adjust_fractal_parameters(int key, t_data *data)
{
	if (key == K_Z)
		switch_palette_prev(data);
	else if (key == K_X)
		switch_palette_next(data);
	else if (key == K_O)
		adjust_c_imag(data, 0.01);
	else if (key == K_I)
		adjust_c_imag(data, -0.01);
	else if (key == K_K)
		adjust_c_real(data, 0.01);
	else if (key == K_L)
		adjust_c_real(data, -0.01);
	else if (key == K_NUM_7 || key == K_NUM_9
		|| key == K_NUM_4 || key == K_NUM_6
		|| key == K_NUM_1 || key == K_NUM_3)
		adjust_base_color_component(data, key);
	else if (key == K_NUM_8 || key == K_NUM_5 || key == K_NUM_2)
		reset_base_color_component(data, key);
}

void	handle_reset_and_exit(int key, t_data *data)
{
	if (key == K_R)
	{
		reset_view(data);
		data->redraw = 1;
	}
	else if (key == K_ESC || key == K_Q)
		exit_fractol(data);
}

void	handle_movement(int key, t_data *data)
{
	if (key == K_S || key == K_DOWN)
		move_offset(data, 0, MOVE_FACTOR);
	else if (key == K_W || key == K_UP)
		move_offset(data, 0, -MOVE_FACTOR);
	else if (key == K_D || key == K_RIGHT)
		move_offset(data, MOVE_FACTOR, 0);
	else if (key == K_A || key == K_LEFT)
		move_offset(data,- MOVE_FACTOR, 0);
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

void	handle_iter_adjustment(int key, t_data *data)
{
	if (key == K_F)
	{
		if (data->max_iter - ITER_STEP >= MIN_ITER)
			data->max_iter -= ITER_STEP;
		else
			data->max_iter = MIN_ITER;
		data->redraw = 1;
	}
	else if (key == K_G)
	{
		if (data->max_iter + ITER_STEP <= MAX_ITER)
			data->max_iter += ITER_STEP;
		else
			data->max_iter = MAX_ITER;
		data->redraw = 1;
	}
}
