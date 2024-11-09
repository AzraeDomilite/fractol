#include "../include/fractol.h"
#include "../include/keys.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define ZOOM_IN 1.1
#define ZOOM_OUT 0.9
#define MOVE_STEP 20
#define MAX_ITER 100

typedef enum e_fractal_type	{
	JULIA,
	MANDELBROT,
	TRICORN
} t_fractal_type;

typedef struct s_data	{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	double			zoom;
	double			offset_x;
	double			offset_y;
	double			c_real;
	double			c_imag;
	int				redraw;
	t_fractal_type	fractal_type;
} t_data;

void		exit_fractol(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	exit(EXIT_SUCCESS);
}

void		reset_view(t_data *data)
{
	data->zoom			= 1.0;
	data->offset_x		= 0;
	data->offset_y		= 0;
	data->c_real		= -0.7;
	data->c_imag		= 0.27015;
	data->redraw		= 1;
}

int		deal_key(int key, t_data *data)
{
	if (key == K_ESC || key == K_Q)
		exit_fractol(data);
	else if (key == K_R)
		reset_view(data);
	else if (key == K_S || key == K_DOWN)
	{
		data->offset_y += MOVE_STEP;
		data->redraw = 1;
	}
	else if (key == K_W || key == K_UP)
	{
		data->offset_y -= MOVE_STEP;
		data->redraw = 1;
	}
	else if (key == K_D || key == K_RIGHT)
	{
		data->offset_x += MOVE_STEP;
		data->redraw = 1;
	}
	else if (key == K_A || key == K_LEFT)
	{
		data->offset_x -= MOVE_STEP;
		data->redraw = 1;
	}
	else if (key == K_NUM_PLUS)
	{
		data->zoom *= ZOOM_IN;
		data->redraw = 1;
	}
	else if (key == K_NUM_MINUS)
	{
		data->zoom *= ZOOM_OUT;
		data->redraw = 1;
	}
	ft_printf("Key pressed: %d\n", key);
	return (0);
}

void		put_pixel(t_data *data, int x, int y, int color)
{
	char		*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

int		get_color(int iter)
{
	double				t;
	int				r;
	int				g;
	int				b;

	t = (double)iter / MAX_ITER;
	r = (int)(9 * (1 - t) * t * t * t * 255);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((r << 16) | (g << 8) | b);
}

void		draw_julia(t_data *data)
{
	int				x, y, iter;
	double			z_real, z_imag, tmp;
	double			scale = 4.0 / (WIN_WIDTH * data->zoom);
	double			real_min = (-WIN_WIDTH / 2.0 + data->offset_x) * scale;
	double			imag_min = (-WIN_HEIGHT / 2.0 + data->offset_y) * scale;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			z_real = real_min + x * scale;
			z_imag = imag_min + y * scale;
			iter = 0;
			while (z_real * z_real + z_imag * z_imag <= 4 && iter < MAX_ITER)
			{
				tmp = z_real * z_real - z_imag * z_imag + data->c_real;
				z_imag = 2.0 * z_real * z_imag + data->c_imag;
				z_real = tmp;
				iter++;
			}
			put_pixel(data, x, y, get_color(iter));
			x++;
		}
		y++;
	}
}

void		draw_mandelbrot(t_data *data)
{
	int				x, y, iter;
	double			c_real, c_imag, z_real, z_imag, tmp;
	double			scale = 4.0 / (WIN_WIDTH * data->zoom);

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			c_real = (-WIN_WIDTH / 2.0 + data->offset_x + x) * scale;
			c_imag = (-WIN_HEIGHT / 2.0 + data->offset_y + y) * scale;
			z_real = 0;
			z_imag = 0;
			iter = 0;
			while (z_real * z_real + z_imag * z_imag <= 4 && iter < MAX_ITER)
			{
				tmp = z_real * z_real - z_imag * z_imag + c_real;
				z_imag = 2.0 * z_real * z_imag + c_imag;
				z_real = tmp;
				iter++;
			}
			put_pixel(data, x, y, get_color(iter));
			x++;
		}
		y++;
	}
}

void		draw_tricorn(t_data *data)
{
	int				x, y, iter;
	double			c_real, c_imag, z_real, z_imag, tmp;
	double			scale = 4.0 / (WIN_WIDTH * data->zoom);

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			c_real = (-WIN_WIDTH / 2.0 + data->offset_x + x) * scale;
			c_imag = (-WIN_HEIGHT / 2.0 + data->offset_y + y) * scale;
			z_real = 0;
			z_imag = 0;
			iter = 0;
			while (z_real * z_real + z_imag * z_imag <= 4 && iter < MAX_ITER)
			{
				tmp = z_real * z_real - z_imag * z_imag + c_real;
				z_imag = -2.0 * z_real * z_imag + c_imag;
				z_real = tmp;
				iter++;
			}
			put_pixel(data, x, y, get_color(iter));
			x++;
		}
		y++;
	}
}

int		ft_strcmp(char *s1, char *s2)
{
	int				i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void		select_fractal(t_data *data, char *fractal_name)
{
	if (ft_strcmp(fractal_name, "julia") == 0)
		data->fractal_type = JULIA;
	else if (ft_strcmp(fractal_name, "mandelbrot") == 0)
		data->fractal_type = MANDELBROT;
	else if (ft_strcmp(fractal_name, "tricorn") == 0)
		data->fractal_type = TRICORN;
	else
	{
		ft_printf("Usage: ./fractol <julia|mandelbrot|tricorn>\n");
		exit(EXIT_FAILURE);
	}
}

int		render_next_frame(t_data *data)
{
	if (!data->redraw)
		return (0);

	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		perror("Error\nImage creation failed");
		exit_fractol(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);

	if (data->fractal_type == JULIA)
		draw_julia(data);
	else if (data->fractal_type == MANDELBROT)
		draw_mandelbrot(data);
	else if (data->fractal_type == TRICORN)
		draw_tricorn(data);

	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	data->redraw = 0;
	return (0);
}

int		mouse_scroll(int button, int x, int y, t_data *data)
{
	double		prev_zoom;

	prev_zoom = data->zoom;
	if (button == 4)
		data->zoom *= ZOOM_IN;
	else if (button == 5)
		data->zoom *= ZOOM_OUT;

	data->offset_x = (WIN_WIDTH / 2 - x) * (1 - data->zoom / prev_zoom) + data->offset_x;
	data->offset_y = (WIN_HEIGHT / 2 - y) * (1 - data->zoom / prev_zoom) + data->offset_y;

	data->redraw = 1;

	return (0);
}

int		main(int argc, char **argv)
{
	t_data			data;

	if (argc != 2)
	{
		ft_printf("Usage: ./fractol <julia|mandelbrot|tricorn>\n");
		return (1);
	}

	data.mlx = mlx_init();
	reset_view(&data);
	data.img = NULL;

	select_fractal(&data, argv[1]);

	data.win = mlx_new_window(data.mlx, WIN_WIDTH, WIN_HEIGHT, "Fract'ol");
	if (!data.win)
	{
		perror("Error\nWindow creation failed");
		exit(EXIT_FAILURE);
	}
	mlx_key_hook(data.win, deal_key, &data);
	mlx_hook(data.win, 17, 0, exit_fractol, &data);
	mlx_hook(data.win, 4, 0, mouse_scroll, &data);
	mlx_loop_hook(data.mlx, render_next_frame, &data);
	mlx_loop(data.mlx);
	perror("Error\nProgram failed on loop");

	exit(EXIT_FAILURE);
}
