/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:59:50 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 14:15:37 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

char	*str_join_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

int	check_escape(double z_real, double z_imag)
{
	return (z_real * z_real + z_imag * z_imag > ESCAPE_RADIUS);
}

void	calculate_next_point(double *z_real, double *z_imag,
		double c_real, double c_imag)
{
	double	tmp;

	tmp = *z_real * *z_real - *z_imag * *z_imag + c_real;
	*z_imag = 2.0 * *z_real * *z_imag + c_imag;
	*z_real = tmp;
}

int	is_in_main_cardioid(double x, double y)
{
	double	p;

	p = sqrt((x - 0.25) * (x - 0.25) + y * y);
	return (x <= p - 2.0 * p * p + 0.25);
}

int	is_in_period2_bulb(double x, double y)
{
	double	squared;

	squared = (x + 1.0) * (x + 1.0) + y * y;
	return (squared <= 0.0625);
}
