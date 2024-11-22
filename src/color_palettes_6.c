/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:56:42 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 18:56:42 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_gradient_data(t_gradient_data *gd, t_gradient_params params)
{
	gd->color.r = 0.0f;
	gd->color.g = 0.0f;
	gd->color.b = 0.0f;
}

int	get_color_logarithmic(int iter, int max_iter, t_data *data)
{
	double	normalized;
	int		color;

	(void)data;
	if (iter < max_iter)
		normalized = log((double)iter) / log((double)max_iter);
	else
		normalized = 0;
	color = (int)(normalized * MAX_COLOR_VALUE);
	return ((color << 16) | (color << 8) | color);
}
