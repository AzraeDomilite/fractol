/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:15:17 by blucken           #+#    #+#             */
/*   Updated: 2024/11/21 15:08:13 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	process_fractional_part(char *frac_str, double frac_part, int precision)
{
	int	i;

	frac_str[0] = '.';
	i = 1;
	while (i <= precision)
	{
		frac_part *= 10;
		frac_str[i] = ((int)frac_part % 10) + '0';
		frac_part -= (int)frac_part;
		i++;
	}
	frac_str[i] = '\0';
}
