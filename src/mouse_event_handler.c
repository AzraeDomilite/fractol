/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_event_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:42:32 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 16:42:32 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	handle_selection(t_mouse_data *m_data, t_data *data)
{
	data->is_selecting = 1;
	data->select_start_x = m_data->x;
	data->select_start_y = m_data->y;
	data->select_end_x = m_data->x;
	data->select_end_y = m_data->y;
}

static void	calculate_mouse_coordinates(t_mouse_data *m_data, t_data *data)
{
	m_data->mouse_re = (m_data->x / (double)WIN_WIDTH - 0.5)
		* 4.0 / data->zoom + data->offset_x;
	m_data->mouse_im = (m_data->y / (double)WIN_HEIGHT - 0.5)
		* 3.0 / data->zoom + data->offset_y;
}

static void	update_zoom_offset(t_mouse_data *m_data, t_data *data)
{
	data->zoom *= m_data->zoom_factor;
	data->offset_x = m_data->mouse_re
		- (m_data->x / (double)WIN_WIDTH - 0.5) * 4.0 / data->zoom;
	data->offset_y = m_data->mouse_im
		- (m_data->y / (double)WIN_HEIGHT - 0.5) * 3.0 / data->zoom;
	data->redraw = 1;
}

int	mouse_press(int button, int x, int y, t_data *data)
{
	t_mouse_data	m_data;

	m_data.x = x;
	m_data.y = y;
	if (button == 1)
		handle_selection(&m_data, data);
	else if (button == 4 || button == 5)
	{
		m_data.zoom_factor = (button == 4) ? ZOOM_FACTOR : 1.0 / ZOOM_FACTOR;
		calculate_mouse_coordinates(&m_data, data);
		update_zoom_offset(&m_data, data);
	}
	return (0);
}