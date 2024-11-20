/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 19/11/2024 22:00:29 by blucken           #+#    #+#             */
/*   Updated: 19/11/2024 22:00:29 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	handle_fast_mode(t_data *data)
{
	data->fast_mode = !data->fast_mode;
	data->redraw = 1;
	if (data->fast_mode)
		ft_printf("Mode Rapide Activé\n");
	else
		ft_printf("Mode Rapide Désactivé\n");
}

void	handle_overlay(t_data *data)
{
	data->overlay_enabled = !data->overlay_enabled;
	data->redraw = 1;
	if (data->overlay_enabled)
		ft_printf("Overlay Activé\n");
	else
		ft_printf("Overlay Désactivé\n");
}

void	handle_screen_mode(t_data *data)
{
	if (data->is_fullscreen)
		exit_fullscreen(data);
	else
		enter_fullscreen(data);
	data->redraw = 1;
}

void	enter_fullscreen(t_data *data)
{
	if (data->is_fullscreen)
		return ;
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	data->win = mlx_new_window(data->mlx, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT, "Fract'ol - Plein Écran");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	data->width = FULLSCREEN_WIDTH;
	data->height = FULLSCREEN_HEIGHT;
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->is_fullscreen = 1;
	render_next_frame(data);
}

void	exit_fullscreen(t_data *data)
{
	if (!data->is_fullscreen)
		return ;
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	data->win = mlx_new_window(data->mlx, data->original_width, data->original_height, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, data->original_width, data->original_height);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	data->width = data->original_width;
	data->height = data->original_height;
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->is_fullscreen = 0;
	render_next_frame(data);
}
