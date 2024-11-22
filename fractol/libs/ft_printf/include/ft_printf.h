/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memalloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:47:56 by blucken           #+#    #+#             */
/*   Updated: 2024/10/22 19:47:56 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *str, ...);
int		ft_formats(va_list args, char flag, int counter);
int		ft_putchar_fd_p(char c, int fd, int counter);
int		ft_putstr_fd_p(char *s, int fd, int counter);
int		ft_atoi_p(const char *str);
int		ft_putnbr_fd_p(int nb, int fd, int counter);
int		ft_putchar_hex_fd_p(int nb, int hex_case, int fd, int counter);
int		ft_int_to_hex_fd_p(unsigned long nb, int hex_case, int fd, int counter);
int		ft_putadd_fd_p(void *ptr, int fd, int counter);
int		ft_unsigned_fd_p(unsigned int nb, int fd, int counter);

#endif