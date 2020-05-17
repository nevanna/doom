#include "engine.h"

/** net thread
*** net thread
*** net thread
*** net thread
*** net thread */

int	ft_compare(t_data *tmp, t_data *client)
{
	if (tmp->x - client[0].x != 0 || tmp->y - client[0].y != 0)
	{
		ft_memcpy(&client[0], tmp, sizeof(t_data));
		ft_bzero(tmp, sizeof(t_data));
		return (1);
	}
	else
		ft_bzero(tmp, sizeof(t_data));
	return (0);
}

/* the same function for server and client */
void	ft_loop_net(t_engine *e)
{
	int	rez_recv;
	SDL_mutex *mutex;

	mutex = SDL_CreateMutex();
	ft_putendl("loop net");
	while (1)
	{
		if (SDLNet_TCP_Send(e->client, &e->data_my[0], 8) > 0)
			ft_putendl("i sent");
		rez_recv = SDLNet_TCP_Recv(e->client, &e->data_tmp[0], 8);
		if (rez_recv > 0)
		{
			ft_putendl("recive cycle");
			SDL_LockMutex(mutex);
			if (ft_compare(&e->data_tmp[0], &e->data_another[0]) == 1)
				e->net_fl = 'T'; //means Transmitte
			else
				e->net_fl = 'S'; //means Same
			SDL_UnlockMutex(mutex);
		}
		SDL_Delay(2000);
	}
	SDL_DestroyMutex(mutex);
}

/** net thread
*** net thread
*** net thread
*** net thread
*** net thread */
int		ft_connect(void *data)
{
	t_engine	*e;

	e = (t_engine *)data;
	while (1)
	{
		ft_putendl("i am in the second thread");
		if (e->host_fl == 'I')
			ft_net_server(e);
		else if (e->client_fl == 'R')  //means ready
			ft_net_client(e);
		else if (e->menu_fl == 'G')
			return (0);
		SDL_Delay(400);
	}
	return (0);
}
