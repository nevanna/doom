#include "doom.h"

static int	ft_identical_sectors(t_sector_lst *f, t_sector_lst *s)
{
	t_points	*fp;
	t_points	*sp;
	int			flag;

	if (ft_sec_length(f) != ft_sec_length(s))
		return (0);
	fp = f->head;
	while (fp)
	{
		flag = 0;
		sp = s->head;
		while (sp)
		{
			if (sp->x == fp->x && sp->y == fp->y)
				flag++;
			sp = sp->next;
		}
		if (flag != 1)
			return (0);
		fp = fp->next;
	}
	return (1);
}

void		ft_copy_sec_data(t_sector_lst *to, t_sector_lst *from, t_doom *d)
{
	t_sector_lst	*to_tmp;

	while (from)
	{
		to_tmp = to;
		while (to_tmp)
		{
			if (ft_identical_sectors(to_tmp, from))
			{
				fprintf(d->log_fd, "COPIED FROM %p TO %p\n", from, to_tmp);
				to_tmp->floor = from->floor;
				to_tmp->ceiling = from->ceiling;
				to_tmp->floor_tex = from->floor_tex;
				to_tmp->floor_tex = from->ceiling;
				to_tmp->valid = from->valid;
			}
			to_tmp = to_tmp->next;
		}
		from = from->next;
	}
}
