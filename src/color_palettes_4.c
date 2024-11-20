/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:55:04 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 19:55:23 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color_custom_interior(int iter, int max_iter,
		t_data *data, double z_real, double z_imag)
{
	t_color_interior	vars;

	if (iter >= max_iter)
		return (COLOR_BLACK);
	calculate_magnitude(z_real, z_imag, &vars.magnitude_sq, &vars.magnitude_val);
	vars.escape_value = log(vars.magnitude_sq) * vars.magnitude_val / 65536.0;
	vars.angle = calculate_angle(z_real, z_imag);
	calculate_uv_components(vars.angle, 1.0, &vars.u, &vars.v);
	yuv_to_rgb(fmax(0.0, 1.0 - vars.escape_value / 4.0),
		vars.u, vars.v, &vars.r, &vars.g, &vars.b);
	return (apply_base_color(data, vars.r, vars.g, vars.b));
}

void	dwell_gradient(t_gradient_params params, const int *counts,
			unsigned char *pixel, t_data *data)
{
	t_gradient_data			gd;
	t_gradient_calc_params	calc_params;
	int						k;
	t_color					color_temp;

	init_gradient_data(&gd, params);
	if (counts[params.height * params.i + params.j] == params.maxiter)
	{
		color_temp.r = MAX_COLOR_VALUE;
		color_temp.g = MAX_COLOR_VALUE;
		color_temp.b = MAX_COLOR_VALUE;
	}
	else
	{
		calc_params = init_calc_params(params.maxiter, 4.0, 1.0, 0.1,
				(double)params.i + (double)params.j * I);
		process_gradient(&gd, calc_params, counts, params);
		color_temp.r = (int)(gd.color.r * MAX_COLOR_VALUE);
		color_temp.g = (int)(gd.color.g * MAX_COLOR_VALUE);
		color_temp.b = (int)(gd.color.b * MAX_COLOR_VALUE);
	}
	k = (params.width * params.j + params.i) * 3;
	set_pixel_colors(pixel, k, &color_temp);
}

int	get_color_dwell_gradient(int iter, int max_iter, t_data *data,
		double z_real, double z_imag)
{
	t_gradient_data			gd;
	t_gradient_calc_params	params;
	double _Complex			gradient;

	params = init_calc_params(max_iter, ESCAPE_RADIUS, 1.0, 0.1,
			z_real + z_imag * I);
	gradient = m_dwell_gradient(params);
	gd.vz = creal(gradient);
	gd.hue = fmod((gd.vz * 360.0), 360.0);
	gd.saturation = 1.0;
	if (iter < max_iter)
		gd.value = 1.0;
	else
		gd.value = 0.0;
	hsv2rgb((float)(gd.hue / 360.0), (float)gd.saturation,
			(float)gd.value, &gd.color.r, &gd.color.g, &gd.color.b);
	return (((int)(gd.color.r * data->base_color.r) << 16) |
			((int)(gd.color.g * data->base_color.g) << 8) |
			(int)(gd.color.b * data->base_color.b));
}

double	m_continuous_dwell(int N, double R, double _Complex c)
{
	int				iter;
	double _Complex	z;

	iter = 0;
	z = 0 + 0 * I;
	while (iter < N)
	{
		z = cpow(z, 2) + c;
		if (cabs(z) > R)
			break ;
		iter++;
	}
	return ((double)iter);
}

double _Complex	m_dwell_gradient(t_gradient_calc_params params)
{
	t_gradient_data	gd;

	gd.m = m_continuous_dwell(params.N, params.R, params.c);
	gd.mx = m_continuous_dwell(params.N, params.R, params.c + params.d);
	gd.my = m_continuous_dwell(params.N, params.R, params.c + params.d * I);
	gd.vx = gd.m - gd.mx;
	gd.vy = gd.m - gd.my;
	gd.vz = params.s;
	gd.vm = sqrt(gd.vx * gd.vx + gd.vy * gd.vy + gd.vz * gd.vz);
	return (gd.vz / gd.vm);
}
