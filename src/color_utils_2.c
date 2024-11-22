/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:08:19 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:09:54 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	hsv_to_rgb(double h, int *r, int *g, int *b)
{
	t_hsv	hsv;

	hsv.c = HSV_VALUE * HSV_SATURATION;
	h = fmod(h, HSV_HUE_MAX) / 60.0;
	hsv.x = hsv.c * (1 - fabs(fmod(h, 2) - 1));
	hsv.hi = (int)h;
	hsv.m = HSV_VALUE - hsv.c;
	apply_rgb_values(hsv, r, g, b);
	scale_rgb_values(r, g, b);
}

void	lch_to_rgb(t_lch_color *lch, int *r, int *g, int *b)
{
	t_color_vars	vars;

	vars.h = lch->h_deg * M_PI / 180.0;
	*r = (int)(MAX_COLOR_VALUE * (lch->l
				/ 100.0 + lch->c * cos(vars.h) / 100.0));
	*g = (int)(MAX_COLOR_VALUE * (lch->l
				/ 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0
				+ 0.866 * lch->c * sin(vars.h) / 100.0));
	*b = (int)(MAX_COLOR_VALUE * (lch->l
				/ 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0
				- 0.866 * lch->c * sin(vars.h) / 100.0));
	if (*r < 0)
		*r = 0;
	else if (*r > MAX_COLOR_VALUE)
		*r = MAX_COLOR_VALUE;
	if (*g < 0)
		*g = 0;
	else if (*g > MAX_COLOR_VALUE)
		*g = MAX_COLOR_VALUE;
	if (*b < 0)
		*b = 0;
	else if (*b > MAX_COLOR_VALUE)
		*b = MAX_COLOR_VALUE;
}


void	yuv_to_rgb(t_color_interior vars, int *r, int *g, int *b)
{
	double	r_f;
	double	g_f;
	double	b_f;
	double	y;

	y = fmax(0.0, 1.0 - vars.escape_value / 4.0);
	r_f = y + 1.407 * vars.v;
	g_f = y - 0.344 * vars.u - 0.714 * vars.v;
	b_f = y + 1.770 * vars.u;
	r_f = fmin(fmax(r_f, 0.0), 1.0);
	g_f = fmin(fmax(g_f, 0.0), 1.0);
	b_f = fmin(fmax(b_f, 0.0), 1.0);
	*r = (int)(r_f * 255.0);
	*g = (int)(g_f * 255.0);
	*b = (int)(b_f * 255.0);
}

void	hsv2rgb(t_gradient_data gd)
{
	t_hsv_to_rgb	hsv;

	if (gd.saturation == 0)
	{
		hsv.r = gd.value;
		hsv.g = gd.value;
		hsv.b = gd.value;
	}
	else
	{
		gd.hue = 6 * (gd.hue - floorf(gd.hue));
		hsv.case_value = (int)floorf(gd.hue);
		calculate_hsv_components(gd, &hsv.p, &hsv.q, &hsv.t);
		get_rgb_from_case(hsv, &hsv.r, &hsv.g, &hsv.b);
	}
	assign_rgb_values(hsv, &gd.color.r, &gd.color.g, &gd.color.b);
}
