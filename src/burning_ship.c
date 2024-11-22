/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burning_ship.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:33:15 by blucken           #+#    #+#             */
/*   Updated: 2024/11/21 15:58:55 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_burning_ship(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	tmp;

	vars->z_real = 0;
	vars->z_imag = 0;
	iter = 0;
	while ((vars->z_real * vars->z_real + vars->z_imag * \
			vars->z_imag <= ESCAPE_RADIUS) && (iter < iter_count))
	{
		vars->z_real = fabs(vars->z_real);
		vars->z_imag = fabs(vars->z_imag);
		tmp = vars->z_real * vars->z_real - vars->z_imag * vars->z_imag
			+ vars->c_real;
		vars->z_imag = 2.0 * vars->z_real * vars->z_imag + vars->c_imag;
		vars->z_real = tmp;
		iter++;
	}
	return (iter);
}
