/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:11:54 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 14:11:54 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	int	color;

	color = get_color_part1(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	color = get_color_part2(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	color = get_color_part3(iter, data, z_real, z_imag, max_iter);
	if (color)
		return (color);
	return (COLOR_BLACK);
}

int	get_color_part1(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_FIRE)
		return (get_color_fire(iter, max_iter, data));
	if (data->palette_type == PALETTE_STRIPES)
		return (get_color_stripes(iter, data));
	if (data->palette_type == PALETTE_SMOOTH)
		return (get_color_smooth(iter, data));
	if (data->palette_type == PALETTE_CLASSIC)
		return (get_color_classic(iter, data));
	if (data->palette_type == PALETTE_DERIVATIVE_BAILOUT)
		return (get_color_derivative_bailout(iter, z_real, z_imag, data));
	if (data->palette_type == PALETTE_LOGARITHMIC)
		return (get_color_logarithmic(iter, max_iter, data));
	return (0);
}

int	get_color_part2(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_HSV)
		return (get_color_hsv(iter, max_iter, data));
	if (data->palette_type == PALETTE_GRADIENT)
		return (get_color_gradient(iter, max_iter, data));
	if (data->palette_type == PALETTE_BLACK_WHITE)
		return (get_color_black_white(iter, data));
	if (data->palette_type == PALETTE_ESCAPE_TIME)
		return (get_color_escape_time(iter, max_iter, data));
	if (data->palette_type == PALETTE_CONTINUOUS_POTENTIAL)
		return (get_color_continuous_potential(iter, z_real,
				z_imag, max_iter, data));
	if (data->palette_type == PALETTE_INTERIOR_DISTANCE)
		return (get_color_interior_distance(iter, z_real,
				z_imag, max_iter, data));
	return (0);
}

int	get_color_part3(int iter, t_data *data, double z_real,
		double z_imag, int max_iter)
{
	if (data->palette_type == PALETTE_LCH)
		return (get_color_lch(iter, max_iter, data));
	if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_NO_SHADING)
		return (get_color_exp_cyclic_lch_no_shading(iter, max_iter, data));
	if (data->palette_type == PALETTE_EXP_CYCLIC_LCH_SHADING)
		return (get_color_exp_cyclic_lch_shading(iter, max_iter, data));
	if (data->palette_type == PALETTE_DWELL_GRADIENT)
		return (get_color_dwell_gradient(iter, max_iter, data,
				z_real, z_imag));
	if (data->palette_type == PALETTE_CUSTOM_INTERIOR)
		return (get_color_custom_interior(iter, max_iter, data,
				z_real, z_imag));
	return (0);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	if (x >= 0 && x < data->width && y >= 0 && y < data->height)
	{
		int	index = y * data->line_len + x * (data->bpp / 8);
		*((unsigned int *)(data->addr + index)) = color;
	}
}
