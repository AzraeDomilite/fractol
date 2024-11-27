/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_drawing_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:31:16 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:33:31 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	draw_info_strings(t_data *data)
{
	int		y;

	if (!data->overlay_enabled)
		return ;
	y = 10;
	mlx_string_put(data->mlx, data->win, 10, y, COLOR_WHITE,
		"Fract'ol - 42 Project");
	y += 20;
	draw_controls(data, &y);
	draw_parameters(data, &y);
}

void	draw_controls(t_data *data, int *y)
{
	mlx_string_put(data->mlx, data->win, 10, *y,
		COLOR_WHITE, "Press R to reset view");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Use arrow keys or WASD to move");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Adjust iterations with F (down) and G (up)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Change palette with Z (previous) and X (next)");
	*y += 20;
	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Toggle Overlay with H");
	*y += 20;
}

void	draw_parameters(t_data *data, int *y)
{
	draw_zoom_info(data, y);
	draw_iter_info(data, y);
	draw_fractal_type(data, y);
	draw_palette_type(data, y);
}

void	draw_zoom_info(t_data *data, int *y)
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

void	draw_iter_info(t_data *data, int *y)
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
