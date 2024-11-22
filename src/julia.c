/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:06:41 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:06:41 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_julia(t_data *data, t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;
	double	z_real_sq;
	double	z_imag_sq;

	z_imag_sq = 0;
	z_real_sq = 0;
	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
	iter = 0;
	while (z_real_sq + z_imag_sq <= ESCAPE_RADIUS && iter < iter_count)
	{
		z_real_sq = vars->z_real * vars->z_real;
		z_imag_sq = vars->z_imag * vars->z_imag;
		tmp = z_real_sq - z_imag_sq + data->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + data->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}
