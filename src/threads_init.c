/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:18:38 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 13:18:38 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_thread_data(t_data *thread_data, t_data *data,int height_per_thread, int i)
{
	thread_data[i] = *data;
	thread_data[i].y_start = i * height_per_thread;
	thread_data[i].y_end = (i + 1) * height_per_thread;
	if (i == NUM_THREADS - 1)
		thread_data[i].y_end = WIN_HEIGHT;
}

void	cleanup_threads(pthread_t *threads, int i)
{
	while (--i >= 0)
		pthread_join(threads[i], NULL);
}
