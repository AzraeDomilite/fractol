/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:12:55 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 14:12:55 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	calculate_hsv_components(float h, float v, float s, float *p, float *q, float *t)
{
	float	f;

	f = h - floorf(h);
	*p = v * (1 - s);
	*q = v * (1 - (s * f));
	*t = v * (1 - (s * (1 - f)));
}

void	get_rgb_from_case(int case_value, float v, float p, float q, float t, float *r, float *g, float *b)
{
	if (case_value == 0)
		(*r = v, *g = t, *b = p);
	else if (case_value == 1)
		(*r = q, *g = v, *b = p);
	else if (case_value == 2)
		(*r = p, *g = v, *b = t);
	else if (case_value == 3)
		(*r = p, *g = q, *b = v);
	else if (case_value == 4)
		(*r = t, *g = p, *b = v);
	else
		(*r = v, *g = p, *b = q);
}

void	assign_rgb_values(float r, float g, float b, float *red, float *grn, float *blu)
{
	*red = r;
	*grn = g;
	*blu = b;
}
