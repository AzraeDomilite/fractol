/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:21:44 by blucken           #+#    #+#             */
/*   Updated: 2024/11/09 10:24:13 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	exit_win(t_mlx mlx)
{
	mlx_destroy_window(mlx.mlx, mlx.mlx);
	free(mlx.mlx);
	free(mlx.win);
	exit(EXIT_SUCCESS);
}
int	deal_key(int key, t_mlx mlx)
{
	(void)mlx;
	// if (key == K_ESC)
	// 	exit_win(mlx);
	ft_printf("%d\n", key);
	return (0);
}

int	main(void)
{
	t_mlx	mlx;

	mlx.mlx = mlx_init();
	if (!mlx.mlx)
	{
		perror("Error\nprogram init failed\n");
		exit(EXIT_FAILURE);
	}
	mlx.win = mlx_new_window(mlx.mlx, WIN_WIDTH, WIN_HEIGHT, "fract'ol");

	mlx_key_hook(mlx.win, deal_key, &mlx);
	mlx_loop(mlx.mlx);
	perror("Error\nprogram faild on loop\n");
	exit(EXIT_FAILURE);
}
