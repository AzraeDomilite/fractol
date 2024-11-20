/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddhabrot_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:12:57 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 18:12:57 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b)
{
	double	c;
	double	x;
	double	m;
	int		hi;

	c = v * s;
	h = fmod(h, HSV_HUE_MAX) / 60.0;
	x = c * (1 - fabs(fmod(h, 2) - 1));
	hi = (int)h;
	m = v - c;
	if (hi == 0)
		(*r = c + m, *g = x + m, *b = m);
	else if (hi == 1)
		(*r = x + m, *g = c + m, *b = m);
	else if (hi == 2)
		(*r = m, *g = c + m, *b = x + m);
	else if (hi == 3)
		(*r = m, *g = x + m, *b = c + m);
	else if (hi == 4)
		(*r = x + m, *g = m, *b = c + m);
	else
		(*r = c + m, *g = m, *b = x + m);
	*r = (int)(*r * MAX_COLOR_VALUE);
	*g = (int)(*g * MAX_COLOR_VALUE);
	*b = (int)(*b * MAX_COLOR_VALUE);
}

void	lch_to_rgb(t_lch_color *lch, int *r, int *g, int *b)
{
	t_color_vars	vars;

	vars.h = lch->h_deg * M_PI / 180.0;
	*r = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 + lch->c * cos(vars.h) / 100.0));
	*g = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0 + 0.866 * lch->c * sin(vars.h) / 100.0));
	*b = (int)(MAX_COLOR_VALUE * (lch->l / 100.0 - 0.5 * lch->c * cos(vars.h) / 100.0 - 0.866 * lch->c * sin(vars.h) / 100.0));
	*r = MIN(MAX(*r, 0), MAX_COLOR_VALUE);
	*g = MIN(MAX(*g, 0), MAX_COLOR_VALUE);
	*b = MIN(MAX(*b, 0), MAX_COLOR_VALUE);
}

void yuv_to_rgb(double y, double u, double v, int *r, int *g, int *b)
{
	double	r_f;
	double	g_f;
	double	b_f;

	r_f = y + 1.407 * v;
	g_f = y - 0.344 * u - 0.714 * v;
	b_f = y + 1.770 * u;
	r_f = fmin(fmax(r_f, 0.0), 1.0);
	g_f = fmin(fmax(g_f, 0.0), 1.0);
	b_f = fmin(fmax(b_f, 0.0), 1.0);
	*r = (int)(r_f * 255.0);
	*g = (int)(g_f * 255.0);
	*b = (int)(b_f * 255.0);
}

void	hsv2rgb(float h, float s, float v, float *red, float *grn, float *blu)
{
	float	p;
	float	q;
	float	t;
	float	r;
	float	g;
	float	b;
	int		case_value;

	if (s == 0)
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		h = 6 * (h - floorf(h));
		case_value = (int)floorf(h);
		calculate_hsv_components(h, v, s, &p, &q, &t);
		get_rgb_from_case(case_value, v, p, q, t, &r, &g, &b);
	}
	assign_rgb_values(r, g, b, red, grn, blu);
}
