/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:33:58 by blucken           #+#    #+#             */
/*   Updated: 2024/11/20 12:33:58 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	get_color_custom_interior(int iter, int max_iter,
		t_data *data, double z_real, double z_imag)
{
	double	magnitude_sq;
	double	magnitude_val;
	double	escape_value;
	double	angle;
	int		r;
	int		g;
	int		b;

	if (iter >= max_iter)
		return (COLOR_BLACK);
	calculate_magnitude(z_real, z_imag, &magnitude_sq, &magnitude_val);
	escape_value = log(magnitude_sq) * magnitude_val / 65536.0;
	angle = calculate_angle(z_real, z_imag);
	double u;
	double v;
	calculate_uv_components(angle, 1.0, &u, &v);
	yuv_to_rgb(fmax(0.0, 1.0 - escape_value / 4.0), u, v, &r, &g, &b);
	return (apply_base_color(data, r, g, b));
}

void	dwell_gradient(int width, int height, int maxiter, int i, int j, const int *counts, unsigned char *pixel, t_data *data) 
{
	int	k0 = height * i + j;
	float	r = 0.0f, g = 0.0f, b = 0.0f;
	
	if (counts[k0] == maxiter)
	{
		r = g = b = 1.0f;
	}
	else
	{
		double _Complex c = (double)i + (double)j * I;
		double _Complex gradient = m_dwell_gradient(maxiter, 4.0, 1.0, 0.1, c);
		double vz = creal(gradient);
		float	hue = fmod((vz * 360.0), 360.0f) / 360.0f;
		float	saturation = 1.0f;
		float	value = (counts[k0] < maxiter) ? 1.0f : 0.0f;
		hsv2rgb(hue, saturation, value, &r, &g, &b);
		r = (r * data->base_color.r) / MAX_COLOR_VALUE;
		g = (g * data->base_color.g) / MAX_COLOR_VALUE;
		b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	}
	
	int	k = (width * j + i) * 3;
	pixel[k + 0] = fminf(fmaxf(255 * r, 0), 255);
	pixel[k + 1] = fminf(fmaxf(255 * g, 0), 255);
	pixel[k + 2] = fminf(fmaxf(255 * b, 0), 255);
}

int get_color_dwell_gradient(int iter, int max_iter, t_data *data, double z_real, double z_imag)
{
    double _Complex c = z_real + z_imag * I;
    double _Complex gradient = m_dwell_gradient(max_iter, ESCAPE_RADIUS, 1.0, 0.1, c);
    double vz = creal(gradient);
    double hue = fmod((vz * 360.0), 360.0);
    double saturation = 1.0;
    double value = (iter < max_iter) ? 1.0 : 0.0;
    float r, g, b;
    
    hsv2rgb((float)(hue / 360.0), (float)saturation, (float)value, &r, &g, &b);
    r = (r * data->base_color.r) / MAX_COLOR_VALUE;
    g = (g * data->base_color.g) / MAX_COLOR_VALUE;
    b = (b * data->base_color.b) / MAX_COLOR_VALUE;
	return ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
}

double m_continuous_dwell(int N, double R, double _Complex c)
{
	int iter;
	double _Complex z = 0 + 0*I;

	for (iter = 0; iter < N; iter++)
	{
		z = cpow(z, 2) + c;
		if (cabs(z) > R)
			break ;
	}
	return (double)iter;
}

double _Complex	m_dwell_gradient(int N, double R, double s, double d, double _Complex c)
{
	double m  = m_continuous_dwell(N, R, c);
	double mx = m_continuous_dwell(N, R, c + d);
	double my = m_continuous_dwell(N, R, c + d * I);
	double vx = m - mx;
	double vy = m - my;
	double vz = s;
	double vm = sqrt(vx * vx + vy * vy + vz * vz);
	return vz / vm;
}