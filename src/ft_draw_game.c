#include "engine.h"

void	musicFinished()
{
	printf("Music stopped.\n");
}

void	ft_fill_for_g(t_engine *e)
{
	e->menu_fl = 'G';
	e->mouse_fl = 'G';
	e->net_fl = 'N';
	e->host_fl = 'N';
	e->client_fl = 'N';
}

void	ft_draw_game(t_engine *e)
{
	if (!(e->menu_fl == 'D'))
		if (ft_new_load_data(e, "src/maps/map-clear.txt") == 0)
			ft_return("error: load data");
	ft_fill_for_g(e);
	//check about it is the first game
	//ft_draw_intro(e);
}
