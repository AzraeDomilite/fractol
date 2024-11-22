/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.json                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 21:57:00 by blucken           #+#    #+#             */
/*   Updated: 2024/10/23 21:57:00 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int	ft_putchar_fd_p(char c, int fd, int counter)
{
	write(fd, &c, 1);
	counter++;
	return (counter);
}

int	ft_putstr_fd_p(char *s, int fd, int counter)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		counter = ft_putstr_fd_p("(null)", fd, counter);
		return (counter);
	}
	while (s[i])
	{
		counter = ft_putchar_fd_p(s[i], fd, counter);
		i++;
	}
	return (counter);
}

int	ft_atoi_p(const char *str)
{
	int		nb;
	int		sign;
	size_t	i;

	nb = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * sign);
}

int	ft_putnbr_fd_p(int nb, int fd, int counter)
{
	unsigned int	nbr;

	if (nb == -2147483648)
	{
		counter = ft_putstr_fd_p("-2147483648", fd, counter);
		return (counter);
	}
	if (nb < 0)
	{
		counter = ft_putchar_fd_p('-', fd, counter);
		nbr = (unsigned int)(nb * -1);
	}
	else
		nbr = (unsigned int)nb;
	if (nbr >= 10)
		counter = ft_putnbr_fd_p(nbr / 10, fd, counter);
	counter = ft_putchar_fd_p((nbr % 10 + 48), fd, counter);
	return (counter);
}

int	ft_putchar_hex_fd_p(int nb, int hex_case, int fd, int counter)
{
	char	*list_lower;
	char	*list_upper;

	list_lower = "0123456789abcdef";
	list_upper = "0123456789ABCDEF";
	if (hex_case == 1)
		write(fd, &list_lower[nb], 1);
	else if (hex_case == 2)
		write(fd, &list_upper[nb], 1);
	counter++;
	return (counter);
}
