/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 21:57:08 by blucken           #+#    #+#             */
/*   Updated: 2024/10/23 21:58:21 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

int	ft_formats(va_list args, char flag, int counter)
{
	if (flag == 'c')
		counter = ft_putchar_fd_p(va_arg(args, int), 1, counter);
	else if (flag == 's')
		counter = ft_putstr_fd_p(va_arg(args, char *), 1, counter);
	else if (flag == 'p')
		counter = ft_putadd_fd_p(va_arg(args, void *), 1, counter);
	else if (flag == 'i' || flag == 'd')
		counter = ft_putnbr_fd_p(va_arg(args, int), 1, counter);
	else if (flag == 'u')
		counter = ft_unsigned_fd_p(va_arg(args, unsigned int), 1, counter);
	else if (flag == 'x')
		counter = ft_int_to_hex_fd_p(va_arg(args, unsigned int), 1, 1, counter);
	else if (flag == 'X')
		counter = ft_int_to_hex_fd_p(va_arg(args, unsigned int), 2, 1, counter);
	else if (flag == '%')
		counter = ft_putchar_fd_p('%', 1, counter);
	return (counter);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		printed_len;
	int		i;

	va_start(args, str);
	printed_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			printed_len = ft_formats(args, str[i + 1], printed_len);
			i++;
		}
		else
			printed_len = ft_putchar_fd_p(str[i], 1, printed_len);
		i++;
	}
	va_end(args);
	return (printed_len);
}
