/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddhabrot_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:38:25 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 18:38:25 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_and_check_point(double *z_real, double *z_imag,
			double c_real, double c_imag)
{
	if (is_in_main_cardioid(c_real, c_imag)
		|| is_in_period2_bulb(c_real, c_imag))
		return ;
	*z_real = 0.0;
	*z_imag = 0.0;
}
