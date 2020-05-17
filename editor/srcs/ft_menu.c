#include "doom.h"

void	ft_redraw(t_doom *d)
{
	ft_init_borders(d);
	ft_clear_surface(d);
	ft_net(d);
	ft_draw_lst(d);
	ft_menu(d);
	ft_draw_cursor_tex(d, d->frame_size * 10);
}

void	ft_bort(t_doom *d)
{
	int x;
	int y;

	y = 0;
	while (y <= d->img.menu.height)
	{
		x = 0;
		while (x <= d->img.menu.width)
		{
			if (y == 0 || y == (d->img.menu.height - 1))
				ft_surface_set_pixel(d->sdl.sur_ptr, x + d->img.net.width, y, d->color.menu_border);
			if (x == 0 || x == (d->img.menu.width - 1))
				ft_surface_set_pixel(d->sdl.sur_ptr, x + d->img.net.width, y, d->color.menu_border);
			x++;
		}
		y++;
	}
}

int			ft_is_but_area(t_button but, int x, int y)
{
	if (x > but.x && x < but.x + but.x_len
			&& y > but.y && y < but.y + but.y_len)
		return (1);
	return (0);
}

static void	ft_clear_menu(t_doom *d)
{
	int		x;
	int		y;

	y = -1;
	while (++y < d->height)
	{
		x = d->width - d->img.menu.width - 1;
		while (++x < d->width)
			ft_surface_set_pixel(d->sdl.sur_ptr, x, y, 0x000000);
	}
}

void	ft_menu(t_doom *d)
{
		ft_clear_menu(d);
		ft_bort(d);
		ft_trash_main(d);
		ft_save_main(d);
		ft_ctrz_main(d);
		ft_textures_left_main(d);
		ft_textures_right_main(d);
		ft_textures_main(d);
}

