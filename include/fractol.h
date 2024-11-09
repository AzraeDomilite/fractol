/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:20:42 by blucken           #+#    #+#             */
/*   Updated: 2024/11/09 10:21:25 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../mlx/mlx.h"
# include "keys.h"
# include "../libs/libft/src/libft.h"
# include "../libs/ft_printf/include/ft_printf.h"
# include <unistd.h>
# include <math.h>
# include <stdlib.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 720

typedef struct s_mlx 
{
	void	*mlx;
	void	*win;
}			t_mlx;

#endif
