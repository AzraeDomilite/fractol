/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:17:42 by blucken           #+#    #+#             */
/*   Updated: 2024/11/22 17:17:42 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_lyap_vars(t_lyap_vars *vars, int iter_count)
{
	vars->value = 0.5;
	vars->sum_log_deriv = 0.0;
	vars->max_iter = iter_count * ft_strlen(LYAPUNOV_SEQUENCE);
	vars->sequence = LYAPUNOV_SEQUENCE;
}

int	check_value_bounds(double value, double r)
{
	if (r < 0 || r > 4)
		return (0);
	if (value == 0 || value == 1)
		return (0);
	return (1);
}

int	calculate_final_value(double sum_log_deriv, int iter)
{
	if (iter == 0)
		return (0);
	sum_log_deriv /= iter;
	if (sum_log_deriv > 0)
		return ((int)(sum_log_deriv * MAX_COLOR_VALUE));
	return (0);
}

double	get_r_value(t_fractal_vars *vars, char sequence_char)
{
	if (sequence_char == 'A')
		return (vars->c_real);
	return (vars->c_imag);
}

int	compute_lyapunov(t_fractal_vars *vars, int iter_count)
{
	t_lyap_vars	l_vars;

	init_lyap_vars(&l_vars, iter_count);
	l_vars.iter = 0;
	while (l_vars.iter < l_vars.max_iter)
	{
		l_vars.m = l_vars.iter % ft_strlen(l_vars.sequence);
		l_vars.r = get_r_value(vars, l_vars.sequence[l_vars.m]);
		if (!check_value_bounds(l_vars.value, l_vars.r))
			break ;
		l_vars.value = l_vars.r * l_vars.value * (1 - l_vars.value);
		l_vars.sum_log_deriv += log(fabs(l_vars.r * (1 - 2 * l_vars.value)));
		l_vars.iter++;
	}
	return (calculate_final_value(l_vars.sum_log_deriv, l_vars.iter));
}
