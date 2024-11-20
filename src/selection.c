/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 19/11/2024 22:00:29 by blucken           #+#    #+#             */
/*   Updated: 19/11/2024 22:00:29 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	get_sorted_selection(t_data *data, int *x_start, int *x_end, int *y_start, int *y_end)
{
	*x_start = ft_clamp((data->select_start_x < data->select_end_x) ? data->select_start_x : data->select_end_x, 0, data->width - 1);
	*x_end = ft_clamp((data->select_start_x > data->select_end_x) ? data->select_start_x : data->select_end_x, 0, data->width - 1);
	*y_start = ft_clamp((data->select_start_y < data->select_end_y) ? data->select_start_y : data->select_end_y, 0, data->height - 1);
	*y_end = ft_clamp((data->select_start_y > data->select_end_y) ? data->select_start_y : data->select_end_y, 0, data->height - 1);
}

void	draw_rectangle_edges(t_data *data, int x_start, int y_start, int x_end, int y_end)
{
	int	x;

	for (x = x_start; x <= x_end; x++)
	{
		put_pixel(data, x, y_start, COLOR_WHITE);
		put_pixel(data, x, y_end, COLOR_WHITE);
	}

	int	y;
	for (y = y_start; y <= y_end; y++)
	{
		put_pixel(data, x_start, y, COLOR_WHITE);
		put_pixel(data, x_end, y, COLOR_WHITE);
	}
}

void	swap_int(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	zoom_to_selection(t_data *data)
{
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;
	double	start_x;
	double	start_y;
	double	end_x;
	double	end_y;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return ;
	if (data->fractal_type == BUDDHABROT)
		calculate_buddha_bounds(data, x_start, x_end, y_start, y_end);
	else
	{
		calculate_coordinates(data, x_start, y_start, &start_x, &start_y);
		calculate_coordinates(data, x_end, y_end, &end_x, &end_y);
		update_zoom_and_center(data, start_x, end_x, start_y, end_y);
	}
	data->redraw = 1;
}

void calculate_zoom_and_offset(t_data *data, int x_start, int x_end, int y_start, int y_end)
{
	double x_min;
	double x_max;
	double y_min;
	double y_max;	

	x_min = data->offset_x + (x_start - WIN_WIDTH / 2.0) * (4.0 / (WIN_WIDTH * data->zoom));
	x_max = data->offset_x + (x_end - WIN_WIDTH / 2.0) * (4.0 / (WIN_WIDTH * data->zoom));
	y_min = data->offset_y + (y_start - WIN_HEIGHT / 2.0) * (4.0 / (WIN_HEIGHT * data->zoom));
	y_max = data->offset_y + (y_end - WIN_HEIGHT / 2.0) * (4.0 / (WIN_HEIGHT * data->zoom));	
	update_zoom_and_offset(data, x_min, x_max, y_min, y_max);
}
