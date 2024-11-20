/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:29:14 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:29:14 by blucken          ###   ########.ch       */
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
	hsv_to_rgb(vars.h, HSV_SATURATION, HSV_VALUE, &r, &g, &b);
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
	r = (int)((1 - vars.t) * 0 + vars.t * MAX_COLOR_VALUE);
	g = (int)((1 - vars.t) * 0 + vars.t * 165);
	b = (int)((1 - vars.t) * MAX_COLOR_VALUE + vars.t * 0);
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

int	get_color_continuous_potential(int iter, double z_real,
		double z_imag, int max_iter, t_data *data)
{
	double	zn;
	double	nu;
	double	t;
	int		r;
	int		g;
	int		b;

	zn = sqrt(z_real * z_real + z_imag * z_imag);
	nu = log(log(zn) / log(2)) / log(2);
	t = iter + 1 - nu;
	t = t / max_iter;
	r = (int)(MAX_COLOR_VALUE * t * (data->base_color.r / 255.0));
	g = (int)(MAX_COLOR_VALUE * (1 - t) * (data->base_color.g / 255.0));
	b = (int)(MAX_COLOR_VALUE * (t * (1 - t)) * (data->base_color.b / 255.0));
	return ((r << 16) | (g << 8) | b);
}
