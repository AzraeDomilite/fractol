/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_rgb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:00:29 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 17:00:29 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	adjust_red(t_data *data, int key)
{
	if (key == K_NUM_7)
	{
		if (data->base_color.r - COLOR_ADJUST_STEP < 0)
			data->base_color.r = 0;
		else
			data->base_color.r -= COLOR_ADJUST_STEP;
	}
	else if (key == K_NUM_9)
	{
		if (data->base_color.r + COLOR_ADJUST_STEP > MAX_COLOR_VALUE)
			data->base_color.r = MAX_COLOR_VALUE;
		else
			data->base_color.r += COLOR_ADJUST_STEP;
	}
}

static void	adjust_green(t_data *data, int key)
{
	if (key == K_NUM_4)
	{
		if (data->base_color.g - COLOR_ADJUST_STEP < 0)
			data->base_color.g = 0;
		else
			data->base_color.g -= COLOR_ADJUST_STEP;
	}
	else if (key == K_NUM_6)
	{
		if (data->base_color.g + COLOR_ADJUST_STEP > MAX_COLOR_VALUE)
			data->base_color.g = MAX_COLOR_VALUE;
		else
			data->base_color.g += COLOR_ADJUST_STEP;
	}
}

static void	adjust_blue(t_data *data, int key)
{
	if (key == K_NUM_1)
	{
		if (data->base_color.b - COLOR_ADJUST_STEP < 0)
			data->base_color.b = 0;
		else
			data->base_color.b -= COLOR_ADJUST_STEP;
	}
	else if (key == K_NUM_3)
	{
		if (data->base_color.b + COLOR_ADJUST_STEP > MAX_COLOR_VALUE)
			data->base_color.b = MAX_COLOR_VALUE;
		else
			data->base_color.b += COLOR_ADJUST_STEP;
	}
}

void	adjust_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_7 || key == K_NUM_9)
		adjust_red(data, key);
	else if (key == K_NUM_4 || key == K_NUM_6)
		adjust_green(data, key);
	else if (key == K_NUM_1 || key == K_NUM_3)
		adjust_blue(data, key);
	data->redraw = 1;
}
