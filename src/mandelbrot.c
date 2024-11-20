/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burning_ship.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:50:57 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:50:57 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_mandelbrot(t_data *data, t_fractal_vars *vars, int iter_count)
{
	init_mandel_vars(vars);
	if (data->fractal_type == TRICORN)
		return (compute_tricorn(vars, iter_count));
	return (compute_standard(vars, iter_count));
}

static void	init_mandel_vars(t_fractal_vars *vars)
{
	vars->z_real = 0.0;
	vars->z_imag = 0.0;
}

static int	compute_tricorn(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	iter = 0;
	while (iter < iter_count)
	{
		z_real_sq = vars->z_real * vars->z_real;
		z_imag_sq = vars->z_imag * vars->z_imag;
		if (z_real_sq + z_imag_sq > ESCAPE_RADIUS)
			break ;
		tmp = z_real_sq - z_imag_sq + vars->c_real;
		vars->z_imag = -2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}

static int	compute_standard(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	iter = 0;
	while (iter < iter_count)
	{
		z_real_sq = vars->z_real * vars->z_real;
		z_imag_sq = vars->z_imag * vars->z_imag;
		if (z_real_sq + z_imag_sq > ESCAPE_RADIUS)
			break ;
		tmp = z_real_sq - z_imag_sq + vars->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}
