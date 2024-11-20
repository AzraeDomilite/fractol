/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 20/11/2024 12:06:33 by blucken           #+#    #+#             */
/*   Updated: 20/11/2024 12:06:33 by blucken          ###   ########.ch       */
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
		return (compute_lyapunov(vars, iter_count));
	else if (data->fractal_type == NEWTON)
		return (compute_newton(vars, iter_count));
	return (0);
}
