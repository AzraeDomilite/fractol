/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_handlers_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 20/11/2024 12:07:01 by blucken           #+#    #+#             */
/*   Updated: 20/11/2024 12:07:01 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	reset_base_color_component(t_data *data, int key)
{
	if (key == K_NUM_8)
		data->base_color.r = MAX_COLOR_VALUE;
	else if (key == K_NUM_5)
		data->base_color.g = MAX_COLOR_VALUE;
	else if (key == K_NUM_2)
		data->base_color.b = MAX_COLOR_VALUE;
	data->redraw = 1;
}
