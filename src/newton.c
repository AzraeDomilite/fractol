/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:19:46 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:19:46 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_newton(t_fractal_vars *vars, int iter_count)
{
	int		iter;
	double	old_real;
	double	old_imag;
	double	denominator;

	init_newton_vars(vars);
	iter = 0;
	while (iter < iter_count)
	{
		old_real = vars->z_real;
		old_imag = vars->z_imag;
		denominator = calculate_denominator(old_real, old_imag);
		if (denominator == 0)
			break ;
		calculate_next_z(vars, old_real, old_imag, denominator);
		if (check_convergence(old_real, old_imag,
				vars->z_real, vars->z_imag))
			break ;
		iter++;
	}
	return (iter);
}

void	init_newton_vars(t_fractal_vars *vars)
{
	vars->z_real = vars->c_real;
	vars->z_imag = vars->c_imag;
}

double	calculate_denominator(double old_real, double old_imag)
{
	double	diff_squared;

	diff_squared = old_real * old_real - old_imag * old_imag;
	return (3.0 * diff_squared * diff_squared);
}

void	calculate_next_z(t_fractal_vars *vars, double old_real, double old_imag, double denominator)
{
	double	tmp;
	double	real_cubed;
	double	imag_cubed;

	real_cubed = old_real * old_real * old_real;
	imag_cubed = old_imag * old_imag * old_imag;
	tmp = (2.0 * real_cubed - 2.0 * old_real * old_imag * old_imag
			- old_real) / denominator;
	vars->z_imag = (2.0 * imag_cubed - 2.0 * old_real * old_real
			* old_imag + old_imag) / denominator;
	vars->z_real = tmp;
}

int	check_convergence(double old_real, double old_imag, double new_real, double new_imag)
{
	double	diff_real;
	double	diff_imag;

	diff_real = old_real - new_real;
	diff_imag = old_imag - new_imag;
	return ((diff_real * diff_real + diff_imag * diff_imag) < 1e-6);
}