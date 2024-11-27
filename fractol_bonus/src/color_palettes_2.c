/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:34:41 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:34:41 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color_hsv(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	int				r;
	int				g;
	int				b;

	vars.t = (double)iter / max_iter;
	vars.h = vars.t * HSV_HUE_MAX;
	hsv_to_rgb(vars.h, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_gradient(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	int				r;
	int				g;
	int				b;

	vars.t = (double)iter / max_iter;
	r = (int)((1 - vars.t) * GRAD_R_EXT + vars.t * GRAD_R_INT);
	g = (int)((1 - vars.t) * GRAD_G_EXT + vars.t * GRAD_G_INT);
	b = (int)((1 - vars.t) * GRAD_B_EXT + vars.t * GRAD_B_INT);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_black_white(int iter, t_data *data)
{
	if (iter % 2 == 0)
		return ((data->base_color.r << 16) | (data->base_color.g << 8)
			| data->base_color.b);
	return (COLOR_BLACK);
}

int	get_color_escape_time(int iter, int max_iter, t_data *data)
{
	double	t;
	int		color;

	t = (double)iter / max_iter;
	color = (int)(t * MAX_COLOR_VALUE);
	color = (color * data->base_color.r) / MAX_COLOR_VALUE;
	return ((color << 16) | (color << 8) | color);
}

int	get_color_continuous_potential(t_color_args *args)
{
	t_color_potential	vars;

	vars.zn = sqrt(args->z_real * args->z_real + args->z_imag * args->z_imag);
	vars.nu = log(log(vars.zn) / log(2)) / log(2);
	vars.t = args->iter + 1 - vars.nu;
	vars.t = vars.t / args->max_iter;
	vars.r = (int)(MAX_COLOR_VALUE * vars.t
			* (args->data->base_color.r / 255.0));
	vars.g = (int)(MAX_COLOR_VALUE * (1 - vars.t)
			* (args->data->base_color.g / 255.0));
	vars.b = (int)(MAX_COLOR_VALUE * (vars.t * (1 - vars.t))
			* (args->data->base_color.b / 255.0));
	return ((vars.r << 16) | (vars.g << 8) | vars.b);
}
