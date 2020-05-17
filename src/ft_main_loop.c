#include "engine.h"

void	ft_main_loop(t_engine *e, SDL_Thread **thread)
{
	SDL_mutex	*mutex;

	mutex = SDL_CreateMutex();
	while (1)
	{
		while (SDL_PollEvent(&(e->sdl.event)))
		{
			if (e->sdl.event.type == SDL_QUIT ||
				(e->sdl.event.type == SDL_KEYDOWN
					&& e->sdl.event.key.keysym.sym == SDLK_ESCAPE))
				ft_exit_x(e);
			else if (e->sdl.event.type == SDL_KEYDOWN
				|| e->sdl.event.type == SDL_KEYUP)
				ft_ch_key(e);
			else if (e->sdl.event.type == SDL_MOUSEBUTTONDOWN
				&& e->mouse_fl != 'G')
				ft_ch_mouse(e);
		}
		ft_draw_func(e);
	}
	SDL_DestroyMutex(mutex);
}

void	ft_general_loop(t_engine *e)
{
	SDL_Thread	*thread;

	thread = SDL_CreateThread(ft_connect, "second thread", (void *)e);
	if (e->menu_fl == 'G')
	{
		ft_putendl("i will close thread");
		SDL_WaitThread((thread), NULL);
		ft_putendl("i closed thread");
	}
	if (NULL == thread)
		ft_return("error: SDL_CreateThread failed");
	ft_main_loop(e, &thread);
	SDL_WaitThread(thread, NULL);
}

