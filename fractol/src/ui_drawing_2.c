/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_drawing_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:37:20 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:37:20 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	draw_fractal_type(t_data *data, int *y)
{
	const char	*fractal_names[] = {
		"Julia", "Mandelbrot", "Tricorn", "Burning Ship",
		"Newton",
	};

	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Fractal Type: ");
	mlx_string_put(data->mlx, data->win, 120, *y, COLOR_WHITE,
		(char *)fractal_names[data->fractal_type]);
	*y += 20;
}

void	draw_palette_type(t_data *data, int *y)
{
	const char	*palette_names[] = {
		"Fire",
		"Stripes",
		"Smooth",
		"Classic",
		"Logarithmic",
		"HSV",
		"Gradient",
		"Black & White",
		"Escape Time",
		"Continuous Potential",
		"Interior Distance",
		"LCH",
		"Exp Cyclic LCH No Shading",
		"Exp Cyclic LCH Shading",
		"Derivative Bailout",
		"Rainbow Interior"
	};

	mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE,
		"Palette Type: ");
	mlx_string_put(data->mlx, data->win, 120, *y, COLOR_WHITE,
		(char *)palette_names[data->palette_type]);
	*y += 20;
}

void	draw_selection_rectangle(t_data *data)
{
	t_zoom		zoom;
	t_draw_rect	rect;

	get_sorted_selection(data, &zoom);
	if (zoom.x_start == zoom.x_end || zoom.y_start == zoom.y_end)
		return ;
	rect.x_start = zoom.x_start;
	rect.y_start = zoom.y_start;
	rect.x_end = zoom.x_end;
	rect.y_end = zoom.y_end;
	draw_rectangle_edges(data, &rect);
}
