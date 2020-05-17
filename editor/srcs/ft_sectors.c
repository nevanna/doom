#include "doom.h"

static void				ft_print_point_lst(t_points *head, t_doom *d)
{
	t_points	*tmp;

	tmp = head;
	while (tmp)
	{
		fprintf(d->log_fd, "x->%d, y->%d\n", tmp->x, tmp->y);
		tmp = tmp->next;
	}
}

void					ft_print_sector_lst(t_sector_lst *head, t_doom *d)
{
	t_sector_lst	*sec;


	sec = head;
	while (sec)
	{
		fprintf(d->log_fd, "\nSTARTED SECTOR\n");
		fprintf(d->log_fd, "%p\n\n", sec);
		ft_print_point_lst(sec->head, d);
		fprintf(d->log_fd, "valid-->%d\n", sec->valid);
		sec = sec->next;
	}
}

static void				ft_free_sector_lst(t_sector_lst *head)
{
	t_sector_lst	*next;

	while (head)
	{
		next = head->next;
		ft_free_point_lst(head->head);
		free(head);
		head = next;
	}
}

static int				ft_check_repeat(t_sector_lst *tmp, t_points *ptmp)
{
	t_points	*cur;

	if (!tmp || !tmp->head)
		return (0);
	cur = tmp->head;
	if (ptmp->x == cur->x && ptmp->y == cur->y)
		return (2);
	while (cur)
	{
		// fprintf(d->log_fd, "entered\n");
		if (cur->x == ptmp->x && cur->y == ptmp->y)
			return (1);
		cur = cur->next;
	}
	return (0);
}

static int				ft_check_connection(t_doom *d, t_sector_lst *sec, t_points *p)
{
	t_points	*arr;
	t_line		*lines;

	arr = sec->head;
	if (arr == NULL)
		return (1);
	while (arr->next)
		arr = arr->next;
	lines  = d->lines;
	while (lines)
	{
		if (((arr->x == lines->x_s && arr->y == lines->y_s) ||
			(arr->x == lines->x_e && arr->y == lines->y_e)) &&
			((p->x == lines->x_s && p->y == lines->y_s) ||
			(p->x == lines->x_e && p->y == lines->y_e)))
			return (1);
		lines = lines->next;
	}
	return (0);
}

static void				ft_add_point_to_sector(t_sector_lst *lst, t_points *p)
{
	t_points	*tmp;
	t_points	*new;

	tmp = lst->head;
	new = ft_memalloc(sizeof(t_points));
	while (tmp && tmp->next)
		tmp = tmp->next;
	new->x = p->x;
	new->y = p->y;
	if (tmp == NULL)
		lst->head = new;
	else
		tmp->next = new;
	
}

static t_sector_lst		*ft_next_sector(t_sector_lst *lst)
{
	t_points		*old;
	t_points		*new;
	t_sector_lst	*new_lst;

	// fprintf(d->log_fd, "got here\n");
	new_lst = ft_memalloc(sizeof(t_sector_lst));
	lst->next = new_lst;
	new = new_lst->head;
	old = lst->head;
	while (old)
	{
		ft_add_point_to_sector(new_lst, old);
		// fprintf(d->log_fd, "x->%d      y->%d\n", old->x, old->y);
		old = old->next;
	}
	return (new_lst);
}

int					ft_sec_length(t_sector_lst *lst)
{
	t_points	*p;
	int			i;

	i = 0;
	p = lst->head;
	while (p)
	{
		p = p->next;
		i++;
	}
	return (i);
}

static int				ft_sector_repeat(t_sector_lst *head, t_sector_lst *sec)
{
	int				tmp;
	t_sector_lst	*cur_lst;
	t_points		*curp;
	t_points		*secp;

	cur_lst = head;
	while (cur_lst)
	{
		// fprintf(d->log_fd, "kek\n");
		if (ft_sec_length(cur_lst) == ft_sec_length(sec) && cur_lst != sec)
		{
			tmp = 0;
			secp = sec->head;
			while (secp)
			{
				curp = cur_lst->head;
				while (curp)
				{
					if (curp->x == secp->x && curp->y == secp->y)
						tmp++;
					curp = curp->next;
				}
				secp = secp->next;
			}
			if (tmp == ft_sec_length(sec))
				return (1);
		}
		cur_lst = cur_lst->next;
	}
	return (0);
}

static int				ft_is_part_of_list(t_points	*head, int x, int y)
{
	while (head)
	{
		if (x == head->x && y == head->y)
			return (1);
		head = head->next;
	}
	return (0);
}

static int				ft_vector_product(double x1, double y1, double x2, double y2)
{
	double	res;

	res = x1 * y2 - y1 * x2;
	if (res > 0)
		return (1);
	else if (res < 0)
		return (-1);
	return (0);
}

static double			ft_angle_vectors(double x1, double y1, double x2, double y2)
{

	return (ft_vector_product(x1, y1, x2, y2) * acos((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 +
		y1 * y1) * sqrt(x2 * x2 + y2 * y2))));
}

static int ft_check_inside_point(t_points *head_pts, t_points *allp, t_sector_lst *lst, t_doom *d)
{
	t_points	*curp;
	t_points	*secp;
	double		res;

	curp = allp;
	while (curp)
	{
		secp = lst->head;
		res = 0;
		if (!ft_is_part_of_list(lst->head, curp->x, curp->y))
		{
			while (secp->next)
			{
				res += ft_angle_vectors(secp->x - curp->x, secp->y - curp->y,
					secp->next->x - curp->x, secp->next->y - curp->y);
				secp = secp->next;
			}
			res += ft_angle_vectors(secp->x - curp->x, secp->y - curp->y,
				lst->head->x - curp->x, lst->head->y - curp->y);
			// if (fabs(1 - res / 2) < fabs(res / 2))
			fprintf(d->log_fd, " \nx->%d, y->%d    %f\n",curp->x, curp->y, res);
			if (fabs(res) > 0.1)
			{
				return (1);
			}
		}
		curp = curp->next;
	}
	return (0);
}


static int				ft_check_final(t_sector_lst *head, t_sector_lst *lst, t_points *ptmp, t_doom *d)
{
	t_points	*tmp;

	tmp = lst->head;
	if (tmp == NULL)
		return (0);
	if (ft_sec_length(lst) < 3 || !(tmp->x == ptmp->x && tmp->y == ptmp->y))
		return (0);
	fprintf(d->log_fd, "\nThis one got in final\n\n");
	ft_print_point_lst(lst->head, d);
	if ((ft_sector_repeat(head, lst)))
	{
		fprintf(d->log_fd, "\nfailed because of repeat\n\n");
		return (0);
	}
	if ((ft_check_inside_point(tmp, d->point_lst, lst, d))) //&&
		// !(ft_lines_inside_sector(tmp, d->lines)))
		{
			fprintf(d->log_fd, "\nfailed because of inside point\n\n");
			return (0);
		}
	return (1);
}

static void				ft_lst_del_last(t_sector_lst *tmp)
{
	t_points	*head;
	t_points	*prev;

	prev = NULL;
	if (tmp)
	{
		head = tmp->head;
		while (head->next)
		{
			prev = head;
			head = head->next;
		}
		free(head);
		if (prev != NULL)
			prev->next = NULL;
		else
		{
			// if (head)
				// free(head);
			tmp->head = NULL;
		}
	}
}

static void				ft_sec_recursion(t_doom *d, t_sector_lst *head, t_sector_lst **tmp)
{
	t_points	*ptmp;
	int			got;
	int			rep;

	// fprintf(d->log_fd, "entered recursion\n");
	got = 0;
	ptmp = d->point_lst;
	while (ptmp)
	{
		// fprintf(d->log_fd, "in cycle\n");
		if ((rep = ft_check_repeat(*tmp, ptmp)) != 1)
		{
			// fprintf(d->log_fd, "check repeat\n");
			if ((ft_check_connection(d, *tmp, ptmp)))
			{
				// fprintf(d->log_fd, "check_connection\n");
				if (rep == 2 && ft_check_final(head, *tmp, ptmp, d))
				{
					// fprintf(d->log_fd, "check final\n");
					*tmp = ft_next_sector(*tmp);
				}
				else if (rep == 0)
				{
					ft_add_point_to_sector(*tmp, ptmp);
					// fprintf(d->log_fd, "add point\n");
					ft_sec_recursion(d, head, tmp);
					ft_lst_del_last(*tmp);
				}
				// ft_print_point_lst(tmp->head);
				// fprintf(d->log_fd, "new recursion\n");
			}
		}
		ptmp = ptmp->next;
	}
	// fprintf(d->log_fd, "ended recursion\n");
	// if (got > 0)
	// {
	// 	ft_lst_del_last(tmp);
	// 	fprintf(d->log_fd, "del");
	// }
}

static int				ft_check_repeated_points(t_points *toch, t_points *ch)
{
	int				flag;
	t_points		*tmp;

	while (ch)
	{
		tmp = toch;
		flag = 0;
		while (tmp && !flag)
		{
			if (ch->x == tmp->x && ch->y == tmp->y)
				flag++;
			tmp = tmp->next;
		}
		if (flag == 0)
			return (0);
		ch = ch->next;
	}
	return (1);
}

static int				ft_check_little_sectors(t_points *toch, t_points *ch1, t_points *ch2)
{
	t_points	*tmp;
	int			flag;

	while (toch)
	{
		flag = 0;
		tmp = ch1;
		while (tmp)
		{
			if (tmp->x == toch->x && tmp->y == toch->y)
				flag++;
			tmp = tmp->next;
		}
		tmp = ch2;
		while (tmp)
		{
			if (tmp->x == toch->x && tmp->y == toch->y)
				flag++;
			tmp = tmp->next;
		}
		if (flag == 0)
			return (0);
		toch = toch->next;
	}
	return (1);
}

static int				ft_can_change(t_points *toch, t_points *ch1, t_points *ch2)
{
	int			points;
	t_points	*tmp;
	int			flag;

	if (ft_check_repeated_points(toch, ch1) &&
		ft_check_repeated_points(toch, ch2))
	{
		// if (ft_check_little_sectors(toch, ch1, ch2))
			return (1);
	}
	return (0);
}

static void				ft_del_sect(t_sector_lst **head, t_sector_lst *todel, t_doom *d)
{
	t_sector_lst	*prev;
	t_sector_lst	*newhead;
	t_sector_lst	*tmp;

	prev = NULL;
	tmp = *head;
	while (tmp != todel)
		tmp = tmp->next;
	if (tmp == *head)
	{
		fprintf(d->log_fd, "DELETED %p\n", tmp);
		*head = (*head)->next;
		ft_free_point_lst(tmp->head);
		free(tmp);
	}
	else
	{
		fprintf(d->log_fd, "DELETED %p\n", tmp);
		prev = *head;
		while (prev->next != tmp)
			prev = prev->next;
		prev->next = tmp->next;
		ft_free_point_lst(tmp->head);
		free(tmp);
	}
}

static void				ft_clear_big_sectors(t_sector_lst **sec, t_doom *d)
{
	t_sector_lst	*first;
	t_sector_lst	*second;
	t_sector_lst	*poten;

	poten = *sec;
	while (poten)
	{
		first = *sec;
		while (first)
		{
			if (first != poten)
			{
				second = *sec;
				while (second)
				{
					// fprintf(d->log_fd, "second\n");
					if (second != first && second != poten && first != poten)
						if (ft_can_change(poten->head, first->head,
							second->head))
							{
								ft_del_sect(sec, poten, d);
								poten = *sec;
							}
					second = second->next;
				}
			}
			first = first->next;
		}
		// fprintf(d->log_fd, "lolkek\n");
		poten = poten->next;
	}
}

static void				ft_free_last_sec(t_sector_lst **head)
{
	t_sector_lst	*prev;
	t_sector_lst	*tmp;

	if (!(*head))
		return ;
	prev = NULL;
	tmp = *head;
	while (tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp->head)
		ft_free_point_lst(tmp->head);
	if (tmp == *head)
	{
		free(tmp);
		(*head) = NULL;
		return ;
	}
	free(tmp);
	prev->next = NULL;
}

static void				ft_find_sectors(t_doom *d)
{
	t_sector_lst	*head;
	t_points		*phead;
	t_sector_lst	*tmp;

	phead = d->point_lst;
	// head = ft_memalloc(sizeof(t_sector_lst));
	tmp = ft_memalloc(sizeof(t_sector_lst));
	head = tmp;
	ft_sec_recursion(d, head, &tmp);
	ft_free_last_sec(&head);
	fprintf(d->log_fd, "BEFORE\n");
	// ft_print_sector_lst(head, d);
	ft_clear_big_sectors(&head, d);
	ft_copy_sec_data(head, d->sect_head, d);
	ft_print_sector_lst(head, d);
	ft_free_sector_lst(d->sect_head);
	d->sect_head = head;
	fprintf(d->log_fd, "AFTER\n");
}

void					ft_check_sectors(t_doom *d)
{
	int			num;

	ft_transform_to_points(d, &num);
	// ft_print_point_lst(d->point_lst);
	ft_find_sectors(d);
	d->flags |= (1 << 2);
	ft_fill_sector_info(d);
	d->flags &= ~(1 << 2);
	d->flags = 0;
	if (d->sect_head)
		ft_print_sector_lst(d->sect_head, d);
}
