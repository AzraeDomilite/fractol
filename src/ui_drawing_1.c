/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:45:18 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:45:18 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	draw_info_strings(t_data *data)
{
	int		y;

	if (!data->overlay_enabled)
		return ;
	y = 10;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, "Fract'ol - 42 Project");
	y += 20;
	draw_controls(data, &y);
	draw_parameters(data, &y);
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, data->fast_mode ? "Mode Rapide: Activé" : "Mode Rapide: Désactivé");
	y += 20;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE, data->is_fullscreen ? "Plein Écran: Activé" : "Plein Écran: Désactivé");
	y += 20;
}


void	draw_controls(t_data *data, int *y)
{
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Press R to reset view");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Use arrow keys or WASD to move");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Use + and - to zoom");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Adjust iterations with F (down) and G (up)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Change palette with Z (previous) and X (next)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Fast Mode with 1");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Overlay with H");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, "Toggle Fullscreen with 0");
	*y += 20;
}

void	draw_parameters(t_data *data, int *y)
{
	draw_zoom_info(data, y);
	draw_iter_info(data, y);
	draw_rgb_info(data, y);
	draw_fractal_type(data, y);
	draw_palette_type(data, y);
}

static void	draw_zoom_info(t_data *data, int *y)
{
	char	*str;
	char	*temp;

	temp = ft_ftoa(data->zoom, 2);
	str = ft_strjoin("Zoom: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;
}

static void	draw_iter_info(t_data *data, int *y)
{
	char	*str;
	char	*temp;

	temp = ft_itoa(data->max_iter);
	str = ft_strjoin("Iterations: ", temp);
	free(temp);
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
	free(str);
	*y += 20;
}