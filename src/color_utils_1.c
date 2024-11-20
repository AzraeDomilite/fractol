/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:14:06 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 18:14:06 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color(t_color_args *args)
{
	int	color;

	color = get_color_part1(args);
	if (color)
		return (color);
	color = get_color_part2(args);
	if (color)
		return (color);
	color = get_color_part3(args);
	if (color)
		return (color);
	return (COLOR_BLACK);
}

int	get_color_part1(t_color_args *args)
{
	if (args->data->palette_type == PALETTE_FIRE)
		return (get_color_fire(args->iter, args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_STRIPES)
		return (get_color_stripes(args->iter, args->data));
	if (args->data->palette_type == PALETTE_SMOOTH)
		return (get_color_smooth(args->iter, args->data));
	if (args->data->palette_type == PALETTE_CLASSIC)
		return (get_color_classic(args->iter, args->data));
	if (args->data->palette_type == PALETTE_DERIVATIVE_BAILOUT)
		return (get_color_derivative_bailout(args->iter, args->z_real,
				args->z_imag, args->data));
	if (args->data->palette_type == PALETTE_LOGARITHMIC)
		return (get_color_logarithmic(args->iter, args->max_iter, args->data));
	return (0);
}

int	get_color_part2(t_color_args *args)
{
	if (args->data->palette_type == PALETTE_HSV)
		return (get_color_hsv(args->iter, args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_GRADIENT)
		return (get_color_gradient(args->iter, args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_BLACK_WHITE)
		return (get_color_black_white(args->iter, args->data));
	if (args->data->palette_type == PALETTE_ESCAPE_TIME)
		return (get_color_escape_time(args->iter, args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_CONTINUOUS_POTENTIAL)
		return (get_color_continuous_potential(args));
	if (args->data->palette_type == PALETTE_INTERIOR_DISTANCE)
		return (get_color_interior_distance(args->iter, args->z_real,
				args->z_imag, args->max_iter, args->data));
	return (0);
}

int	get_color_part3(t_color_args *args)
{
	if (args->data->palette_type == PALETTE_LCH)
		return (get_color_lch(args->iter, args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_EXP_CYCLIC_LCH_NO_SHADING)
		return (get_color_exp_cyclic_lch_no_shading(args->iter,
				args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_EXP_CYCLIC_LCH_SHADING)
		return (get_color_exp_cyclic_lch_shading(args->iter,
				args->max_iter, args->data));
	if (args->data->palette_type == PALETTE_DWELL_GRADIENT)
		return (get_color_dwell_gradient(args->iter, args->max_iter,
				args->data, args->z_real, args->z_imag));
	if (args->data->palette_type == PALETTE_CUSTOM_INTERIOR)
		return (get_color_custom_interior(args->iter, args->max_iter,
				args->data, args->z_real, args->z_imag));
	return (0);
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	int	index;

	index = 0;
	if (x >= 0 && x < data->width && y >= 0 && y < data->height)
	{
		index = y * data->line_len + x * (data->bpp / 8);
		*((unsigned int *)(data->addr + index)) = color;
	}
}
