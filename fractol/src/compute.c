/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:04:53 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 18:05:13 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_fractal(t_data *data, t_fractal_vars *vars, int iter_count)
{
	if (data->fractal_type == JULIA)
		return (compute_julia(data, vars, iter_count));
	else if (data->fractal_type == MANDELBROT
		|| data->fractal_type == TRICORN)
		return (compute_mandelbrot(data, vars, iter_count));
	else if (data->fractal_type == BURNING_SHIP)
		return (compute_burning_ship(vars, iter_count));
	else if (data->fractal_type == LYAPUNOV)
		return (compute_newton(vars, iter_count));
	return (0);
}
