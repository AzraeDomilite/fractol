/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 19/11/2024 22:00:29 by blucken           #+#    #+#             */
/*   Updated: 19/11/2024 22:00:29 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_julia(t_data *data, t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
	iter = 0;
	while (((z_real_sq = vars->z_real * vars->z_real)
			+ (z_imag_sq = vars->z_imag * vars->z_imag)) <= ESCAPE_RADIUS
		&& (iter < iter_count))
	{
		tmp = z_real_sq - z_imag_sq + data->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + data->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}
