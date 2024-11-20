/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:58:28 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 16:58:28 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	switch_palette_next(t_data *data)
{
	if (data->palette_type + 1 >= PALETTE_COUNT)
		data->palette_type = 0;
	else
		data->palette_type++;
	data->redraw = 1;
}

void	switch_palette_prev(t_data *data)
{
	if (data->palette_type == 0)
		data->palette_type = PALETTE_COUNT - 1;
	else
		data->palette_type--;
	data->redraw = 1;
}

void	adjust_c_imag(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_imag += delta;
	else
		data->offset_y += delta;
	data->redraw = 1;
}

void	adjust_c_real(t_data *data, double delta)
{
	if (data->fractal_type == JULIA)
		data->c_real += delta;
	else
		data->offset_x += delta;
	data->redraw = 1;
}
