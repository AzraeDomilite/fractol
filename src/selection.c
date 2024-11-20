/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:40:21 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 19:40:21 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	get_sorted_selection(t_data *data, t_zoom *zoom)
{
	if (data->select_start_x < data->select_end_x)
		zoom->x_start = ft_clamp(data->select_start_x, 0, data->width - 1);
	else
		zoom->x_start = ft_clamp(data->select_end_x, 0, data->width - 1);
	if (data->select_start_x > data->select_end_x)
		zoom->x_end = ft_clamp(data->select_start_x, 0, data->width - 1);
	else
		zoom->x_end = ft_clamp(data->select_end_x, 0, data->width - 1);
	if (data->select_start_y < data->select_end_y)
		zoom->y_start = ft_clamp(data->select_start_y, 0, data->height - 1);
	else
		zoom->y_start = ft_clamp(data->select_end_y, 0, data->height - 1);
	if (data->select_start_y > data->select_end_y)
		zoom->y_end = ft_clamp(data->select_start_y, 0, data->height - 1);
	else
		zoom->y_end = ft_clamp(data->select_end_y, 0, data->height - 1);
}

void	draw_rectangle_edges(t_data *data, t_draw_rect *rect)
{
	rect->y = rect->y_start;
	while (rect->y <= rect->y_end)
	{
		put_pixel(data, rect->x_start, rect->y, COLOR_WHITE);
		put_pixel(data, rect->x_end, rect->y, COLOR_WHITE);
		rect->y++;
	}
	rect->x = rect->x_start;
	while (rect->x <= rect->x_end)
	{
		put_pixel(data, rect->x, rect->y_start, COLOR_WHITE);
		put_pixel(data, rect->x, rect->y_end, COLOR_WHITE);
		rect->x++;
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
	t_zoom	zoom;

	get_sorted_selection(data, &zoom);
	if (zoom.x_start == zoom.x_end || zoom.y_start == zoom.y_end)
		return ;
	if (data->fractal_type == BUDDHABROT)
		calculate_buddha_bounds(data, &zoom);
	else
	{
		calculate_coordinates(data, &zoom);
		update_zoom_and_center(data, &zoom);
	}
	data->redraw = 1;
}

void calculate_zoom_and_offset(t_data *data, int x_start, int x_end, int y_start, int y_end)
{
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;	

	x_min = data->offset_x + (x_start - WIN_WIDTH / 2.0)
		* (4.0 / (WIN_WIDTH * data->zoom));
	x_max = data->offset_x + (x_end - WIN_WIDTH / 2.0)
		* (4.0 / (WIN_WIDTH * data->zoom));
	y_min = data->offset_y + (y_start - WIN_HEIGHT / 2.0)
		* (4.0 / (WIN_HEIGHT * data->zoom));
	y_max = data->offset_y + (y_end - WIN_HEIGHT / 2.0)
		* (4.0 / (WIN_HEIGHT * data->zoom));
	update_zoom_and_offset(data, x_min, x_max, y_min, y_max);
}
