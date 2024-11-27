/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 21:36:38 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 21:36:38 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	render_next_frame(t_data *data)
{
	if (!data->redraw)
		return (0);
	if (data->fast_mode)
		draw_fractal_with_iter(data, DEFAULT_PREVIEW_ITER);
	else
		draw_fractal_with_iter(data, data->max_iter);
	data->fast_mode = 0;
	data->redraw = 0;
	return (0);
}

void	draw_fractal_with_iter(t_data *data, int iter_count)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, data->width, data->height);
	if (!data->img)
	{
		ft_printf(ERROR_MSG_IMAGE);
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	if (data->fractal_type == BUDDHABROT)
	{
		data->max_iter = iter_count;
		render_buddhabrot(data);
	}
	else
	{
		draw_fractal(data, iter_count);
	}
	if (data->is_selecting)
		draw_selection_rectangle(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	draw_info_strings(data);
}

void	draw_fractal(t_data *data, int iter_count)
{
	pthread_t	threads[NUM_THREADS];
	t_data		thread_data[NUM_THREADS];
	int			i;
	int			height_per_thread;

	init_fractal_params(data, iter_count);
	height_per_thread = WIN_HEIGHT / NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		init_thread_data(thread_data, data, height_per_thread, i);
		if (pthread_create(&threads[i], NULL, thread_draw_fractal,
				&thread_data[i]) != 0)
		{
			ft_printf(ERROR_THREAD_CREATE);
			cleanup_threads(threads, i);
			exit_fractol(data);
		}
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
}

void	*thread_draw_fractal(void *arg)
{
	t_data				*data;
	t_fractal_params	params;
	t_color_args		color_args;
	int					y;

	(void) color_args;
	data = (t_data *)arg;
	params.real_min = data->real_min;
	params.imag_min = data->imag_min;
	params.scale = data->scale;
	params.iter_count = data->iter_count;
	y = data->y_start;
	while (y < data->y_end)
	{
		draw_fractal_line(data, y, &params);
		y++;
	}
	return (NULL);
}

void	draw_fractal_line(t_data *data, int y, t_fractal_params *params)
{
	t_draw_line	line;

	line.x = 0;
	while (line.x < WIN_WIDTH)
	{
		line.vars.c_real = params->real_min + \
			((double)line.x / WIN_WIDTH) * 4.0 * params->scale;
		line.vars.c_imag = params->imag_min + \
			((double)y / WIN_HEIGHT) * 3.0 * params->scale;
		line.vars.iter = compute_fractal(data, &line.vars, params->iter_count);
		line.args.iter = line.vars.iter;
		line.args.data = data;
		line.args.z_real = line.vars.z_real;
		line.args.z_imag = line.vars.z_imag;
		line.args.max_iter = data->max_iter;
		put_pixel(data, line.x, y, get_color(&line.args));
		line.x++;
	}
}
