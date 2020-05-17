#include "engine.h"

static int				open_file(char *str)
{
	int		fd;
	char	a[1];

	fd = open(str, O_RDONLY);
	if (read(fd, a, 0) == -1)
		fd = 0;
	return (fd);
}

static void				ft_player_data(t_engine *engine, char *gnl)
{
	gnl += 7;
	engine->player.where.x = ft_scan_digit(&gnl);
	engine->player.where.y = ft_scan_digit(&gnl);
	engine->player.angle = ft_scan_digit(&gnl);
	engine->player.sector = ft_scan_digit(&gnl);
	engine->player.where.z = engine->sectors
		[engine->player.sector].floor + EyeHeight;
}

static void				ft_sect_text(t_engine *engine, char *gnl)
{
	char	*save;
	int		i;
	int		j;

	j = -1;
	save = &gnl[7];
	i = ft_scan_digit(&save);
	while (++j < engine->sectors[i].npoints)
		engine->sectors[i].texturs[j] = ft_scan_digit(&save);
}

static int			ft_mnogo_if(t_engine *engine, t_xy **vert, char *gnl)
{
	if (gnl[0] == 'v')
	{
		if (!ft_vert_data(vert, gnl))
			return (0);
	}
	else if (gnl[0] == 's')
	{
		if (!ft_sect_data(engine, *vert, gnl))
		{
			free(*vert);
			return (0);
		}
	}
	else if (gnl[0] == 't')
		ft_sect_text(engine, gnl);
	else if (gnl[0] == 'p')
		ft_player_data(engine, gnl);
	ft_memdel((void **)&gnl);
	return (1);
}

int					ft_new_load_data(t_engine *engine, char *file)
{
	signed char	fd;
	char		*gnl;
	t_xy		*vert;

	if (!(fd = open_file(file)))
		return (fd);
	vert = 0;
	engine->sectors = 0;
	engine->num_sectors = 0;
	while (get_next_line(fd, &gnl))
		if (!(ft_mnogo_if(engine, &vert, gnl)))
			return (0);
	free(vert);
	close(fd);
	return (1);
}
