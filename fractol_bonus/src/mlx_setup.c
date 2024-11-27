/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:11:37 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:45:59 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	initialize_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		ft_printf(ERROR_MLX_INIT);
		exit(EXIT_FAILURE);
	}
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data->win)
	{
		ft_printf(ERROR_MSG_WINDOW);
		exit(EXIT_FAILURE);
	}
}

void	reset_view(t_data *data)
{
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->c_real = INITIAL_C_REAL;
	data->c_imag = INITIAL_C_IMAG;
	data->redraw = 1;
	data->fast_mode = 0;
	data->max_iter = DEFAULT_PREVIEW_ITER;
	data->base_color.r = MAX_COLOR_VALUE;
	data->base_color.g = MAX_COLOR_VALUE;
	data->base_color.b = MAX_COLOR_VALUE;
	data->is_selecting = 0;
}

void	exit_fractol(t_data *data)
{
	pthread_mutex_destroy(&data->histogram_mutex);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		data->mlx = NULL;
	exit(EXIT_SUCCESS);
}
