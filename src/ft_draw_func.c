#include "engine.h"

static void	ft_draw_err(t_engine *e)
{
	e->menu_fl = 'M';
	e->net_err_fl = 'N';
	e->host_fl = 'N';
	e->client_fl = 'N';
	if (e->host_name)
		ft_strclr(e->host_name);
	ft_draw_tired(e);
	ft_menu_net(e);
}

void	ft_draw_func(t_engine *e)
{
	if (e->menu_fl == 'G')
		ft_start_game(e);
	if (e->host_fl == 'Y')
	{
		ft_draw_waiting_list(e);
		e->host_fl = 'I'; //means in host menu;
	}
	else if (e->client_fl == 'Y')
	{
		ft_draw_ask_about_host(e);
		e->client_fl = 'I';
	}
	else if ((e->host_fl == 'N' || e->client_fl == 'N') && e->net_err_fl == 'Y')
	//draw net menu
		ft_draw_err(e);
	else if (e->menu_fl == 'S')
		ft_draw_game(e);
	else if ((e->client_fl == 'C' || e->host_fl == 'C') && e->net_fl == 'T')
		ft_draw_net_game(e);
	else if (e->menu_fl == 'T')
		ft_tab_menu(e);
	SDL_UpdateWindowSurface(e->sdl.win_ptr);
}
