/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:04:31 by blucken           #+#    #+#             */
/*   Updated: 2024/10/20 19:04:31 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int	ft_int_to_hex_fd_p(unsigned long nbr, int hex_case, int fd, int counter)
{
	if (nbr >= 16)
		counter = ft_int_to_hex_fd_p(nbr / 16, hex_case, fd, counter);
	counter = ft_putchar_hex_fd_p(nbr % 16, hex_case, fd, counter);
	return (counter);
}

int	ft_putadd_fd_p(void *ptr, int fd, int counter)
{
	if (!ptr)
	{
		counter = ft_putstr_fd_p("0x0", fd, counter);
		return (counter);
	}
	counter = ft_putstr_fd_p("0x", fd, counter);
	counter = ft_int_to_hex_fd_p((uintptr_t)ptr, 1, fd, counter);
	return (counter);
}

int	ft_unsigned_fd_p(unsigned int nbr, int fd, int counter)
{
	if (nbr >= 10)
		counter = ft_unsigned_fd_p(nbr / 10, fd, counter);
	counter = ft_putchar_fd_p((nbr % 10 + 48), fd, counter);
	return (counter);
}
