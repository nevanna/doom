#include "engine.h"

static void	ft_norm(t_engine *e)
{
	e->host_fl = 'N';
	e->net_err_fl = 'Y';
	e->client_fl = 'N';
	ft_putendl("BACK game button at host/client");
	e->menu_fl = 'M';
	e->net_err_fl = 'N';
	if (e->host_name)
		ft_strclr(e->host_name);
	ft_menu_net(e);
}

void		ft_net_zone(t_engine *e)
{
	if (e->m_x >= 1440 && e->m_x <= 1835 && e->menu_fl == 'M')
	{
		if (e->m_y >= 160 && e->m_y <= 320)
		{
			ft_putendl("HOST game button");
			e->net_fl = 'Y';
			e->host_fl = 'Y';
		}
		else if (e->m_y >= 160 + 200 && e->m_y <= 320 + 200)
		{
			ft_putendl("CLIENT game button");
			e->net_fl = 'Y';
			e->client_fl = 'Y';
		}
		else if (e->m_y >= 160 + 400 && e->m_y <= 320 + 400
			&& e->host_fl == 'N' && e->client_fl == 'N')
		{
			ft_putendl("BACK game button");
			ft_menu_spec(e);
		}
		else if (e->m_y >= 160 + 400 && e->m_y <= 320 + 400
			&& (e->host_fl != 'N' || e->client_fl != 'N'))
			ft_norm(e);
	}
}

void		ft_check_button_zone(t_engine *e)
{
	if (e->m_x >= 1440 && e->m_x <= 1835 && e->menu_fl == 'Y')
	{
		if (e->m_y >= 160 && e->m_y <= 320)
		{
			ft_putendl("START game button");
			e->mouse_fl = 'N';
			e->menu_fl = 'S';
		}
		else if (e->m_y >= 160 + 200 && e->m_y <= 320 + 200)
		{
			ft_putendl("NET game button");
			e->menu_fl = 'M';
			ft_menu_net(e);
		}
		else if (e->m_y >= 160 + 400 && e->m_y <= 320 + 400)
			ft_exit_x(e);
		else if (e->m_y >= 160 + 600 && e->m_y <= 320 + 600)
		{
			/* execute mapeditor */
			ft_putendl("Mapeditor game button");
			system("./maped");
			ft_putendl("Mapeditor game button");
		}
	}
	else
		ft_net_zone(e);
}

void		ft_check_tab_zone(t_engine *e)
{
	if (e->menu_fl == 'D' || e->menu_fl == 'T')
	{
		if (e->m_y >= 160 && e->m_y <= 320)
		{
			ft_putendl("menu");
			//тут что-то надо отфришить чтобы корректно работало
			ft_menu_spec(e);
		}
		else if (e->m_y >= 160 + 200 && e->m_y <= 320 + 200)
		{
			ft_putendl("CONTINUE");
			ft_fill_for_g(e);
			//разобраться что с углом почему прыгает(меняется угол)
			ft_start_game(e);
			//how do we continue game?
		}
		else if (e->m_y >= 160 + 400 && e->m_y <= 320 + 400)
		{
			ft_exit_x(e);
		}
	}
}

int				ft_ch_mouse(t_engine *e)
{
	SDL_GetMouseState(&e->m_x, &e->m_y);
	e->mouse_fl = 'Y';
	if (e->mouse_fl == 'Y' && (e->menu_fl == 'Y' || e->menu_fl == 'M'))
		ft_check_button_zone(e);
	if (e->mouse_fl == 'Y' && (e->menu_fl == 'D' || e->menu_fl == 'T'))
		ft_check_tab_zone(e);
	return (0);
}
