#include "engine.h"

void	ft_load_and_blit(t_engine *e)
{
	e->sdl.destin.x = 0;
	e->sdl.destin.y = 850;
	e->player.health = 100;
	e->player.health_max = 100;
	e->player.ammo_max = 50;
	e->player.ammo = 0;
	e->sdl.img_ptr = IMG_Load("src/images/gui.png");
	if (!e->sdl.img_ptr)
		ft_return("error: img load");
}

void	ft_start_game(t_engine *engine)
{
	if (Mix_PlayingMusic() == 1)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(engine->mus.gmusic);
		engine->mus.gmusic = NULL;
	}
	ft_load_and_blit(engine);
	ft_render(engine);
	ft_mouse(engine);
	ft_col_det(engine);
	SDL_Delay(10);
}
