#include "doom.h"

void	ft_draw_cursor_tex(t_doom *d, int size)
{
	SDL_Surface	*sur;
	int			i;
	int			j;
	int			x;
	int			y;

	if (!(d->flags & 1))
		return ;
	sur = d->textures[d->nb_tex_set];
	SDL_GetMouseState(&x, &y);
	x += 10;
	y += 10;
	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size)
			ft_surface_set_pixel(d->sdl.sur_ptr, x + j, y + i, ft_texture_get_pixel(sur,
				j * sur->w / size, i * sur->h / size, 4));
	}
}
