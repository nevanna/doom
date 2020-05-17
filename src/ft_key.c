#include"engine.h"

void			ft_close_net(t_engine *e)
{
	SDLNet_TCP_Close(e->client);
	if (e->host_fl == 'Y')
		SDLNet_TCP_Close(e->server);
	SDLNet_Quit();
}

void			ft_close(t_engine *e)
{
	if (e->net_fl == 'Y')
		ft_close_net(e);
	if (e->menu_fl == 'Y')
	{
		IMG_Quit();
		TTF_Quit();
		//Deallocate surface
		SDL_FreeSurface(e->sdl.img_ptr);
		e->sdl.img_ptr = NULL;
	}
	if (e->mus.effect)
		Mix_FreeChunk(e->mus.effect);
	if (e->mus.gmusic)
		Mix_FreeMusic(e->mus.gmusic);
	//Destroy window
	SDL_DestroyWindow(e->sdl.win_ptr);
	e->sdl.win_ptr = NULL;
	Mix_Quit();
	SDL_Quit();
}

int				ft_exit_x(t_engine *e)
{
	ft_close(e);
	exit(1);
	return (0);
}

void	ft_tab(t_engine *e)
{
	e->menu_fl = 'T';
	e->mouse_fl = 'Y';
	ft_putendl("tab");
}

int				ft_ch_key(t_engine *e)
{
	if (e->mouse_fl == 'Y' && e->menu_fl == 'M' && e->client_fl == 'I'
		&& e->sdl.event.key.keysym.sym == SDLK_RETURN)
		ft_ready_host_name(e);
	else if (e->mouse_fl == 'Y' && e->menu_fl == 'M' && e->client_fl == 'I'
		&& e->sdl.event.key.keysym.sym == SDLK_BACKSPACE)
		ft_back_space(e);
	else if (e->mouse_fl == 'Y' && e->menu_fl == 'M' && e->client_fl == 'I')
		ft_handle_key(e);
	else if (e->menu_fl == 'G' && e->host_fl == 'N' && e->client_fl == 'N'
		&& e->sdl.event.key.keysym.sym == SDLK_TAB)
		ft_tab(e);
	else if (e->menu_fl == 'G' && e->host_fl == 'N' && e->client_fl == 'N'
		&& e->sdl.event.key.keysym.sym != SDLK_TAB &&
			(e->sdl.event.type == SDL_KEYDOWN ||
			e->sdl.event.type == SDL_KEYUP))
		ft_player_input(e);
	/* не могу вспомнить под что была заготовка */
	// else if (e->menu_fl == 'D' && e->host_fl == 'N' && e->client_fl == 'N'
	// 	&& e->sdl.event.key.keysym.sym == SDLK_TAB)
	// 	{
	// 		ft_putendl("ft_render_game_again");
	// 	}
	return (0);
}
