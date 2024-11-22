/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:09:23 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 18:09:23 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	handle_fast_mode(t_data *data)
{
	data->fast_mode = !data->fast_mode;
	data->redraw = 1;
	if (data->fast_mode)
		ft_printf("Fast Mode Enable\n");
	else
		ft_printf("Fast Mode Disable\n");
}

void	handle_overlay(t_data *data)
{
	data->overlay_enabled = !data->overlay_enabled;
	data->redraw = 1;
	if (data->overlay_enabled)
		ft_printf("Overlay Enable\n");
	else
		ft_printf("Overlay Disable\n");
}
