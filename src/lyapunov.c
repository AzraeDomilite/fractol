/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:52:15 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:52:15 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	compute_lyapunov(t_fractal_vars *vars, int iter_count)
{
	double			value;
	double			sum_log_deriv;
	int				iter;
	int				max_iter;
	double			r;
	unsigned int	m;
	char			*sequence;

	sequence = LYAPUNOV_SEQUENCE;
	init_lyap_vars(&value, &sum_log_deriv, &max_iter, iter_count);
	iter = 0;
	while (iter < max_iter)
	{
		m = iter % ft_strlen(sequence);
		r = get_r_value(vars, sequence[m]);
		if (!check_value_bounds(value, r))
			break ;
		value = r * value * (1 - value);
		sum_log_deriv += log(fabs(r * (1 - 2 * value)));
		iter++;
	}
	return (calculate_final_value(sum_log_deriv, iter));
}

static void	init_lyap_vars(double *value, double *sum_log_deriv,
		int *max_iter, int iter_count)
{
	*value = 0.5;
	*sum_log_deriv = 0.0;
	*max_iter = iter_count * ft_strlen(LYAPUNOV_SEQUENCE);
}

static int	check_value_bounds(double value, double r)
{
	if (r < 0 || r > 4)
		return (0);
	if (value == 0 || value == 1)
		return (0);
	return (1);
}

static int	calculate_final_value(double sum_log_deriv, int iter)
{
	if (iter == 0)
		return (0);
	sum_log_deriv /= iter;
	if (sum_log_deriv > 0)
		return ((int)(sum_log_deriv * MAX_COLOR_VALUE));
	return (0);
}

static double	get_r_value(t_fractal_vars *vars, char sequence_char)
{
	if (sequence_char == 'A')
		return (vars->c_real);
	return (vars->c_imag);
}
