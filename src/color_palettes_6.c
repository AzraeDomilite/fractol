/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:56:42 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 18:56:42 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_gradient_data(t_gradient_data *gd, t_gradient_params params)
{
	gd->color.r = 0.0f;
	gd->color.g = 0.0f;
	gd->color.b = 0.0f;
}
