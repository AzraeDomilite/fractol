/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:59:00 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:59:00 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	ft_swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	ft_clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

char	*ft_ftoa(double n, int precision)
{
	long long	int_part;
	double		frac_part;
	char		*int_str;
	char		*frac_str;

	int_part = (long long)n;
	frac_part = n - (double)int_part;
	if (frac_part < 0)
		frac_part = -frac_part;
	if (!handle_allocation(&int_str, &frac_str, int_part, precision))
		return (NULL);
	process_fractional_part(frac_str, frac_part, precision);
	return (join_and_free(int_str, frac_str));
}

char	*ft_lltoa(long long n)
{
	char		*str;
	int			len;
	int			sign;
	long long	nb;

	sign = (n < 0);
	nb = (n < 0) ? -n : n;
	len = ft_numlen(n);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len-- > sign)
	{
		str[len] = '0' + (nb % 10);
		nb /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

int	ft_numlen(long long n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}
