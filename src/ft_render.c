#include "engine.h"

void	ft_move_player(float dx, float dy, t_engine *engine)
{
	float					px;
	float					py;
	unsigned				s;
	const t_xy				*vert;
	const t_sector			*sect;

	s = 0;
	px = engine->player.where.x;
	py = engine->player.where.y;
	sect = &engine->sectors[engine->player.sector];
	vert = sect->vertex;
	while (s < sect->npoints)
	{
		if (sect->neighbors[s] >= 0 &&
			IntersectBox(px, py, px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) &&
			PointSide(px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) < 0)
		{
			engine->player.sector = sect->neighbors[s];
			break ;
		}
		s++;
	}
	engine->player.where.x += dx;
	engine->player.where.y += dy;
	engine->player.anglesin = sinf(engine->player.angle);
	engine->player.anglecos = cosf(engine->player.angle);
}


void	ft_health(t_engine *e)
{
	e->sdl.text.text = ft_strjoin(ft_strjoin("health ",
		ft_itoa(e->player.health)),
		ft_strjoin("/", ft_itoa(e->player.health_max)));
	if (!e->sdl.text.text)
		ft_return("error: malloc");
	ft_fill_text_struct(&e->sdl.text, e->sdl.destin.x + 230,
		e->sdl.destin.y + 32, 28);
	ft_fill_text_struct_c(&e->sdl.text, 0xff, 0xff, 0xff);
	ft_text(&e->sdl.text, e);
}

void	ft_level(t_engine *e)
{
	e->sdl.text.text = ft_strjoin("level ", ft_itoa(e->player.level));
	ft_fill_text_struct(&e->sdl.text, e->sdl.destin.x + 230,
		e->sdl.destin.y + 145, 18);
	ft_fill_text_struct_c(&e->sdl.text, 0xff, 0xff, 0xff);
	ft_text(&e->sdl.text, e);
}

void	ft_ammo(t_engine *e)
{
	e->sdl.text.text = ft_strjoin(ft_strjoin("ammo ", ft_itoa(e->player.ammo)),
		ft_strjoin("/", ft_itoa(e->player.ammo_max)));
	if (!e->sdl.text.text)
		ft_return("error: malloc");
	ft_fill_text_struct(&e->sdl.text, e->sdl.destin.x + 230,
		e->sdl.destin.y + 95, 22);
	ft_fill_text_struct_c(&e->sdl.text, 0xff, 0xff, 0xff);
	ft_text(&e->sdl.text, e);
}

void	ft_render(t_engine *e)
{
	SDL_LockSurface(e->sdl.sur_ptr);
	ft_draw_screen(e);
	SDL_UnlockSurface(e->sdl.sur_ptr);
	SDL_BlitSurface(e->sdl.img_ptr, NULL, e->sdl.sur_ptr, &(e->sdl.destin));
	ft_level(e);
	ft_ammo(e);
	ft_health(e);
	SDL_UpdateWindowSurface(e->sdl.win_ptr);
}
