/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:46:26 by blucken           #+#    #+#             */
/*   Updated: 2024/11/08 17:11:50 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	deal_key(int key, void *param)
{
	(void)param;
	ft_putchar_fd(key, 1);
	return(0);
}


int	main(void)
{
	void	*mlx;
	void	*win;
	int		x;
	int		y;

	mlx = mlx_init ();
	win = mlx_new_window(mlx, 400, 400, "fract'ol");
	y = 50;
	while (y < 150)
	{
		x = 50;
		while (x < 150)
		{
			mlx_pixel_put(mlx, win, x, y, 0x00AF00FF);
			x++;
		}
		y++;
	}
	mlx_key_hook(win, deal_key, (void *)0);
	mlx_loop(mlx);
}