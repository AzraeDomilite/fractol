/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:01:01 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:01:07 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	apply_rgb_values(t_hsv hsv, int *r, int *g, int *b)
{
	if (hsv.hi == 0 || hsv.hi == 1 || hsv.hi == 2)
		set_rgb_case_012(hsv, r, g, b);
	else
		set_rgb_case_345(hsv, r, g, b);
}

void	set_rgb_case_012(t_hsv hsv, int *r, int *g, int *b)
{
	if (hsv.hi == 0)
	{
		*r = hsv.c + hsv.m;
		*g = hsv.x + hsv.m;
		*b = hsv.m;
	}
	else if (hsv.hi == 1)
	{
		*r = hsv.x + hsv.m;
		*g = hsv.c + hsv.m;
		*b = hsv.m;
	}
	else
	{
		*r = hsv.m;
		*g = hsv.c + hsv.m;
		*b = hsv.x + hsv.m;
	}
}

void	set_rgb_case_345(t_hsv hsv, int *r, int *g, int *b)
{
	if (hsv.hi == 3)
	{
		*r = hsv.m;
		*g = hsv.x + hsv.m;
		*b = hsv.c + hsv.m;
	}
	else if (hsv.hi == 4)
	{
		*r = hsv.x + hsv.m;
		*g = hsv.m;
		*b = hsv.c + hsv.m;
	}
	else
	{
		*r = hsv.c + hsv.m;
		*g = hsv.m;
		*b = hsv.x + hsv.m;
	}
}

void	scale_rgb_values(int *r, int *g, int *b)
{
	*r = (int)(*r * MAX_COLOR_VALUE);
	*g = (int)(*g * MAX_COLOR_VALUE);
	*b = (int)(*b * MAX_COLOR_VALUE);
}
