#include "engine.h"


void	ft_draw_net_game(t_engine *e)
{
	if (Mix_PlayingMusic() == 1)
		Mix_HaltMusic();
	SDL_mutex *mutex;
	mutex = SDL_CreateMutex();
	if (e->client_fl == 'C')
	{
		ft_putendl("i am client");
		e->sdl.img_ptr = IMG_Load("src/images/client_forest_for_server.png");
	}
	else if (e->host_fl == 'C')
	{
		ft_putendl("i am server");
		e->sdl.img_ptr = IMG_Load("src/images/server_forest_for_client.png");
	}
	ft_putendl("kill1");
	ft_putendl(ft_itoa(e->data_another[0].x));
	ft_putendl(ft_itoa(e->data_another[0].y));
	ft_putendl(ft_itoa(e->data_my[0].x));
	ft_putendl(ft_itoa(e->data_my[0].y));
	ft_putendl("kill2");
	SDL_LockMutex(mutex);
	e->net_fl = 'S';
	SDL_UnlockMutex(mutex);
	SDL_DestroyMutex(mutex);
	SDL_BlitSurface(e->sdl.img_ptr, NULL, e->sdl.sur_ptr, NULL);
	SDL_UpdateWindowSurface(e->sdl.win_ptr);
	SDL_Delay(1500);

}
