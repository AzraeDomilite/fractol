/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:31:06 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:31:06 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color_interior_distance(int iter, double z_real,
		double z_imag, int max_iter, t_data *data)
{
	double	distance;
	double	normalized;
	int		color;

	(void)data;
	if (iter == max_iter)
	{
		distance = sqrt(z_real * z_real + z_imag * z_imag);
		normalized = log(distance) / log(ESCAPE_RADIUS);
		color = (int)(normalized * MAX_COLOR_VALUE) % (MAX_COLOR_VALUE + 1);
		return ((color << 16) | (color << 8) | color);
	}
	return (COLOR_BLACK);
}

int	get_color_lch(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int				b;

	vars.t = (double)iter / max_iter;
	lch.l = LCH_L_BASE + LCH_L_RANGE * sin(M_PI * vars.t);
	lch.c = LCH_C_BASE;
	lch.h_deg = HSV_HUE_MAX * vars.t;
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_exp_cyclic_lch_no_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int			b;

	vars.t = exp((double)-iter / max_iter);
	lch.l = LCH_L_BASE;
	lch.c = LCH_C_BASE;
	lch.h_deg = fmod(HSV_HUE_MAX * vars.t * LCH_H_MULTIPLIER, HSV_HUE_MAX);
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_exp_cyclic_lch_shading(int iter, int max_iter, t_data *data)
{
	t_color_vars	vars;
	t_lch_color		lch;
	int				r;
	int				g;
	int				b;

	vars.t = exp((double)-iter / max_iter);
	lch.l = LCH_L_BASE + LCH_L_RANGE * sin(2 * M_PI * vars.t);
	lch.c = LCH_C_BASE + LCH_L_RANGE * sin(2 * M_PI * vars.t);
	lch.h_deg = fmod(HSV_HUE_MAX * vars.t * LCH_H_MULTIPLIER, HSV_HUE_MAX);
	lch_to_rgb(&lch, &r, &g, &b);
	r = (r * data->base_color.r) / MAX_COLOR_VALUE;
	g = (g * data->base_color.g) / MAX_COLOR_VALUE;
	b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((r << 16) | (g << 8) | b);
}

int	get_color_derivative_bailout(int iter, double z_real,
		double z_imag, t_data *data)
{
	double	magnitude;
	int		color;

	(void)data;
	(void)iter;
	magnitude = sqrt(z_real * z_real + z_imag * z_imag);
	color = (int)(MAX_COLOR_VALUE * (1 - exp(-magnitude)));
	return ((color << 16) | (color << 8) | color);
}
