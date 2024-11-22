/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:17:52 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 18:05:44 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "julia") == 0)
		data->fractal_type = JULIA;
	else if (ft_strcmp(argv[1], "mandelbrot") == 0)
		data->fractal_type = MANDELBROT;
	else if (ft_strcmp(argv[1], "tricorn") == 0)
		data->fractal_type = TRICORN;
	else if (ft_strcmp(argv[1], "burningship") == 0)
		data->fractal_type = BURNING_SHIP;
	else if (ft_strcmp(argv[1], "newton") == 0)
		data->fractal_type = NEWTON;
	else
	{
		ft_printf(USAGE_MSG);
		exit(EXIT_FAILURE);
	}
}
