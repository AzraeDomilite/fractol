/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:11:37 by blucken           #+#    #+#             */
/*   Updated: 2024/10/04 15:34:09 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_size(long nb)
{
	size_t	len;

	len = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		len = 1;
	}
	if (nb == 0)
		len = 1;
	else
	{
		while (nb)
		{
			nb = nb / 10;
			len++;
		}
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	len;
	long	nb;
	char	*str;
	int		neg;

	len = count_size((long) n);
	str = (char *) malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	nb = (long) n;
	neg = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		str[0] = '-';
		neg = 1;
	}
	str[len] = '\0';
	while (len > (size_t) neg)
	{
		str[len - 1] = nb % 10 + '0';
		nb = nb / 10;
		len--;
	}
	return (str);
}
