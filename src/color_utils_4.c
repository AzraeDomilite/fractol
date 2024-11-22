/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:03:07 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:03:07 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	calculate_hsv_components(t_gradient_data gd, float *p,
			float *q, float *t)

{
	float	f;

	f = gd.hue - floorf(gd.hue);
	*p = gd.value * (1 - gd.saturation);
	*q = gd.value * (1 - (gd.saturation * f));
	*t = gd.value * (1 - (gd.saturation * (1 - f)));
}

void	get_rgb_from_case(t_hsv_to_rgb hsv, float *r, float *g, float *b)
{
	if (hsv.case_value >= 0 && hsv.case_value <= 2)
		handle_case_0_to_2(hsv, r, g, b);
	else
		handle_case_3_to_5(hsv, r, g, b);
}

void	handle_case_0_to_2(t_hsv_to_rgb hsv, float *r, float *g, float *b)
{
	if (hsv.case_value == 0)
	{
		*r = hsv.case_value;
		*g = hsv.t;
		*b = hsv.p;
	}
	else if (hsv.case_value == 1)
	{
		*r = hsv.q;
		*g = hsv.case_value;
		*b = hsv.p;
	}
	else
	{
		*r = hsv.p;
		*g = hsv.case_value;
		*b = hsv.t;
	}
}

void	handle_case_3_to_5(t_hsv_to_rgb hsv, float *r, float *g, float *b)
{
	if (hsv.case_value == 3)
	{
		*r = hsv.p;
		*g = hsv.q;
		*b = hsv.case_value;
	}
	else if (hsv.case_value == 4)
	{
		*r = hsv.t;
		*g = hsv.p;
		*b = hsv.case_value;
	}
	else
	{
		*r = hsv.case_value;
		*g = hsv.p;
		*b = hsv.q;
	}
}

void	assign_rgb_values(t_hsv_to_rgb hsv, float *red, float *grn, float *blu)
{
	*red = hsv.r;
	*grn = hsv.g;
	*blu = hsv.b;
}
