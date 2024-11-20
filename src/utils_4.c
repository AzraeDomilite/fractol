/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:21:26 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:21:26 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	calculate_coordinates(t_data *data, int x, int y,
		double *coord_x, double *coord_y)
{
	*coord_x = (x / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom
		+ data->offset_x;
	*coord_y = (y / (double)WIN_HEIGHT - 0.5) * 4.0 / data->zoom
		+ data->offset_y;
}

static void	update_zoom_and_center(t_data *data, double start_x, double end_x, double start_y, double end_y)
{
	double	selected_width;
	double	selected_height;
	double	zoom_factor_x;
	double	zoom_factor_y;
	double	new_center_x;
	double	new_center_y;

	selected_width = fabs(end_x - start_x);
	selected_height = fabs(end_y - start_y);
	zoom_factor_x = 4.0 / (selected_width * data->zoom);
	zoom_factor_y = 4.0 / (selected_height * data->zoom);
	data->zoom *= fmin(zoom_factor_x, zoom_factor_y);
	new_center_x = start_x + selected_width / 2.0;
	new_center_y = start_y + selected_height / 2.0;
	data->offset_x = new_center_x;
	data->offset_y = new_center_y;
}

void update_zoom_and_offset(t_data *data, double x_min, double x_max, double y_min, double y_max)
{
	double new_width = x_max - x_min;
	double new_height = y_max - y_min;
	double zoom_ratio_x;
	double zoom_ratio_y;
	double new_zoom;

	zoom_ratio_x = WIN_WIDTH / new_width;
	zoom_ratio_y = WIN_HEIGHT / new_height;
	new_zoom = fmin(zoom_ratio_x, zoom_ratio_y);
	data->zoom *= new_zoom;
	data->offset_x = (x_min + x_max) / 2.0;
	data->offset_y = (y_min + y_max) / 2.0;
}

static char	*join_and_free(char *int_str, char *frac_str)
{
	char	*result;

	result = ft_strjoin(int_str, frac_str);
	free(int_str);
	free(frac_str);
	return (result);
}

static int	handle_allocation(char **int_str, char **frac_str, long long int_part, int precision)
{
	*int_str = ft_lltoa(int_part);
	if (!(*int_str))
		return (0);
	*frac_str = (char *)malloc(precision + 2);
	if (!(*frac_str))
	{
		free(*int_str);
		return (0);
	}
	return (1);
}
