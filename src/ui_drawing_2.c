/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:46:49 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:46:49 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	draw_rgb_info(t_data *data, int *y)
{
	char	*str;

	str = create_rgb_string(data);
	if (str)
	{
		mlx_string_put(data->mlx, data->win, 10, *y, COLOR_WHITE, str);
		free(str);
	}
	*y += 20;
}

static char	*create_rgb_string(t_data *data)
{
	char	*str;

	str = ft_strjoin("RGB: (", ft_itoa(data->base_color.r));
	if (!str)
		return (NULL);
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.g));
	str = str_join_free(str, ft_strdup(", "));
	str = str_join_free(str, ft_itoa(data->base_color.b));
	str = str_join_free(str, ft_strdup(")"));
	return (str);
}

void	draw_fractal_type(t_data *data, int *y)
{
	const char	*fractal_names[] = {
		"Julia", "Mandelbrot", "Tricorn", "Burning Ship",
		"Lyapunov", "Newton", "Buddhabrot"
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
		"Dwell Gradient",
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
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;

	get_sorted_selection(data, &x_start, &x_end, &y_start, &y_end);
	if (x_start == x_end || y_start == y_end)
		return;
	draw_rectangle_edges(data, x_start, y_start, x_end, y_end);
}
