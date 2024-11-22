/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lyapunov.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:18:01 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:18:01 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	set_color(t_color *color, float value)
{
	color->r = value;
	color->g = value;
	color->b = value;
}

t_gradient_calc_params	init_calc_params(int maxiter, double R,
							double _Complex c)
{
	t_gradient_calc_params	params;

	params.n = maxiter;
	params.r = R;
	params.s = DWELL_S;
	params.d = DWELL_D;
	params.c = c;
	return (params);
}

void	process_gradient(t_gradient_data *gd, t_gradient_calc_params params,
			const int *counts, t_gradient_params gparams)
{
	gd->gradient = m_dwell_gradient(params);
	gd->vz = creal(gd->gradient);
	gd->hue = fmod((gd->vz * 360.0), 360.0);
	gd->saturation = 1.0;
	if (counts[gparams.height * gparams.i + gparams.j] < gparams.maxiter)
		gd->value = 1.0;
	else
		gd->value = 0.0;
	hsv2rgb(*gd);
}

void	apply_base_color_dwell(t_color *color, t_data *data)
{
	color->r = (color->r * data->base_color.r) / MAX_COLOR_VALUE;
	color->g = (color->g * data->base_color.g) / MAX_COLOR_VALUE;
	color->b = (color->b * data->base_color.b) / MAX_COLOR_VALUE;
}

void	set_pixel_colors(unsigned char *pixel, int k, t_color *color)
{
	pixel[k + 0] = fminf(fmaxf(255 * color->r, 0), 255);
	pixel[k + 1] = fminf(fmaxf(255 * color->g, 0), 255);
	pixel[k + 2] = fminf(fmaxf(255 * color->b, 0), 255);
}
