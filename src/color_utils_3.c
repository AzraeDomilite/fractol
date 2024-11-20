/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:12:38 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:12:38 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	calculate_magnitude(double z_real, double z_imag, double *magnitude_sq, double *magnitude_val)
{
	*magnitude_sq = z_real * z_real + z_imag * z_imag;
	*magnitude_val = sqrt(*magnitude_sq);
}
static double	calculate_angle(double z_real, double z_imag)
{
	double	angle;

	angle = atan2(z_imag, z_real);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

static int	calculate_color(unsigned int value, unsigned int max_value)
{
	double	normalized;
	int		color;

	normalized = log(1 + value) / log(1 + max_value);
	color = (int)(normalized * 255);
	return ((color << 16) | (color << 8) | color);
}

static void	calculate_uv_components(double angle, double saturation, double *u, double *v)
{
	*u = cos(angle) * saturation;
	*v = sin(angle) * saturation;
}

static int	apply_base_color(t_data *data, int r, int g, int b)
{
	r = (int)((double)r * data->base_color.r / MAX_COLOR_VALUE);
	g = (int)((double)g * data->base_color.g / MAX_COLOR_VALUE);
	b = (int)((double)b * data->base_color.b / MAX_COLOR_VALUE);
	return ((r << 16) | (g << 8) | b);
}