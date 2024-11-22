/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:51:02 by blucken           #+#    #+#             */
/*   Updated: 2024/11/21 15:51:02 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color_custom_interior(t_color_args *args)
{
	t_color_interior	vars;

	if (args->iter >= args->max_iter)
		return (COLOR_BLACK);
	calculate_magnitude(args->z_real, args->z_imag,
		&vars.magnitude_sq, &vars.magnitude_val);
	vars.escape_value = log(vars.magnitude_sq) * vars.magnitude_val / 65536.0;
	vars.angle = calculate_angle(args->z_real, args->z_imag);
	calculate_uv_components(vars.angle, 1.0, &vars.u, &vars.v);
	yuv_to_rgb(vars, &vars.r, &vars.g, &vars.b);
	return (apply_base_color(args->data, vars.r, vars.g, vars.b));
}
