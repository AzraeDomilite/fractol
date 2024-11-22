/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:09:19 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:09:19 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	parse_arguments(argc, argv, &data);
	initialize_mlx(&data);
	reset_view(&data);
	mlx_hook(data.win, 2, 1L << 0, deal_key, &data);
	mlx_hook(data.win, 17, 0, (int (*)())exit_fractol, &data);
	mlx_mouse_hook(data.win, mouse_press, &data);
	mlx_hook(data.win, 6, 1L << 6, mouse_move, &data);
	mlx_hook(data.win, 5, 1L << 3, mouse_release, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);
	ft_printf(ERROR_MSG_LOOP);
	exit(EXIT_FAILURE);
}
