/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanfred <cmanfred@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 19:06:18 by htillman          #+#    #+#             */
/*   Updated: 2019/05/31 23:29:36 by cmanfred         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include "pthread.h"
# include "SDL.h"

typedef struct	s_bmp
{
	Uint16		bf_type;
	Uint32		bf_size;
	Uint32		bf_reserved;
	Uint32		bf_off_bits;
	Uint32		bi_size;
	Uint32		bi_width;
	Uint32		bi_height;
	Uint16		bi_planes;
	Uint16		bi_bit_count;
	Uint32		bi_compression;
	Uint32		bi_size_image;
	Uint32		bi_x_pels_per_meter;
	Uint32		bi_y_pels_per_meter;
	Uint32		bi_color_used;
	Uint32		bi_color_important;
}				t_bmp;

typedef	struct	s_sdl
{
	SDL_Window	*win_ptr;
	SDL_Surface	*sur_ptr;
	SDL_Event	event;
	int			width;
	int			height;
}				t_sdl;

typedef struct	s_paint
{
	void		*img_ptr;
	char		*adr;
	int			width;
	int			height;
	int			bitspp;
	int			size_line;
	int			endian;

}				t_paint;

typedef struct	s_img
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_paint		net;
	t_paint		menu;
}				t_img;



typedef struct	s_tex
{
	int			x_lim;
	int			y_lim;
	int			bord_x;
	int			bord_y;
	int			y_tmp;
	int			x_tmp;
	int			new_x_tmp;
	int			tex;
	int			x;
	int			y;


	int			trey;
	int			t;
}				t_tex;

typedef struct	s_vector
{
	int			x;
	int			y;
}				t_vector;

typedef struct	s_points
{
	int			x;
	int			y;
	struct s_points	*next;
}				t_points;

typedef struct	s_sector_lst
{
	t_points	*head;
	int			floor;
	int			ceiling;
	int			floor_tex;
	int			ceil_tex;
	int			valid;
	struct s_sector_lst	*next;
}				t_sector_lst;



typedef struct	s_b
{
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			d;
	int			d1;
	int			d2;
	int			x;
	int			y;
	int			i;
}				t_b;

typedef struct	s_color
{
	Uint32		set;
	Uint32		draw;

	Uint32		menu;
	Uint32		menu_border;

}				t_color;

typedef struct	s_button
{
	int			x_len;
	int			y_len;
	int			x;
	int			y;
	int			color;
	char		*type;
}				t_button;

typedef struct	s_line
{
	int			x_s;
	int			y_s;
	int			x_e;
	int			y_e;
	t_button	but;
	char		*name; /* название прямой А0 */
	char		type;  /*  */
	int			texture;  /* номер текстуры */
	int			damage; /* дамаг при столкновении */

	struct s_line *next;
	struct s_line *prev;
}				t_line;

typedef struct	s_sec_menu
{
	t_button	ok;
	t_button	fl_num;
	t_button	fl_tex;
	t_button	fl_right;
	t_button	fl_left;
	t_button	ce_num;
	t_button	ce_tex;
	t_button	ce_right;
	t_button	ce_left;
	t_button	border;
	int			num_fl_tex;
	int			num_ce_tex;
	int			ce_high;
	int			fl_high;

}				t_sec_menu;

typedef struct	s_doom
{
	FILE		*log_fd;
	int			width;
	int			height;
	char		*titel;
	int			c;
	double		scale;

	int			border;
	int			frame_size;

	int			x_s;
	int			y_s;
	int			x_e;
	int			y_e;
	t_color		color;
	t_img		img;
	SDL_Surface	**textures;
	SDL_Surface	**bmps;
	int			nb_tex;
	int			nb_tex_cost;
	int			nb_tex_off;
	int			nb_tex_set;
	int			tex_size;
	t_button 	*but;
	int			fd;
	int			nb_ln;
	int			part_ln;
	t_line		*lines;
	t_line		*tail;
	int			flag_scroll;
	t_sdl		sdl;
	int			flags;
	int			mouse_tex;
	t_button	*tex_but;
	t_points	*point_lst;
	t_sector_lst	*sect_head;
	t_sector_lst	*prev_sect;
	t_sec_menu		smenu;
}				t_doom;


void			ft_image_set_pixel(t_paint *image, int x, int y, int color);
int				ft_get_pixel(t_paint *image, int x, int y);
void			ft_read_textures(t_doom *d);
void			ft_nb_tex(t_doom *d);
void			ft_init_borders(t_doom *d);
//
//NIKITA BLOCK
int				ft_exit_x(t_doom *d);
int				ft_ch_key(t_doom *d);
int				ft_flag_draw(t_doom *f);
int				ft_mv_flag(t_doom *f);
void			ft_change_net_scale(t_doom *d);
void			ft_redraw(t_doom *d);
void			ft_move_lst(t_doom *d);
void			ft_draw_lst(t_doom *d);
void			ft_change_lst_coordinates(t_doom *d, int x, int y);
void			ft_draw_start_end(t_doom *d, int size);
SDL_Surface		*ft_bmp_read(t_doom *d, char *file);
void			ft_put_textures_to_menu(t_doom *d);
int				ft_is_but_area(t_button but, int x, int y);
Uint32			ft_texture_get_pixel(SDL_Surface *surface, int x, int y, int antialising);
void			ft_wall_texture(t_doom *d, int frame_size, t_line *wall);
void			ft_draw_cursor_tex(t_doom *d, int size);
int				ft_intersect(t_doom *d);
void			ft_pick_texture(t_doom *d);
void			ft_check_sectors(t_doom *d);
int				ft_get_node_num(t_line *head, t_line *line);
t_line			*ft_get_node_by_num(t_line *head, int num);
void			ft_transform_to_points(t_doom *d, int *num);
void			ft_free_point_lst(t_points *head);
int				ft_check_inter(int a, int b, int c, int d);
int				ft_area(t_vector a, t_vector b, t_vector c);
int				ft_area_sec(t_points *a, t_points *b, t_points *c);
void			ft_print_sector_lst(t_sector_lst *head, t_doom *d);
int				ft_sec_length(t_sector_lst *lst);
void			ft_copy_sec_data(t_sector_lst *to, t_sector_lst *from, t_doom *d);
void			ft_fill_sector_info(t_doom *d);
void			ft_draw_sec_menu(t_doom *d, t_sector_lst *sec);
void			ft_init_menu(t_doom *d, t_sector_lst *sec);
void			ft_draw_nbr(t_doom *d, int num, int off, t_button *but);
void			ft_change_num(t_doom *d, int *num);
void			ft_change_sec_coordinates(t_sector_lst *head, int x, int y);
void			ft_change_ceil_texture(t_doom *d, t_sector_lst *lst, int num);
void			ft_change_floor_texture(t_doom *d, t_sector_lst *lst, int num);
//

//t_img		*ft_delimage(t_img *img);
//int			ft_get_pixel(t_img *image, int x, int y);
//void		ft_new_image(t_img *img);

void			ft_net(t_doom *d);
void			ft_menu(t_doom *d);
void			ft_brasenham(t_doom *k, char flag, int col);
void			ft_clear_net(t_doom *d);
void			ft_put_point(t_doom *f);

//buttons
void			ft_button(t_doom *d, t_button *tmp);
void			ft_init_but_2(t_button *b, int y, int color);
void			ft_init_but(t_button *b, int x_len, int y_len, int x);
void			ft_what_but(t_doom *d, t_button *b);

void			ft_trash_main(t_doom *d);
void			ft_trash(t_doom *d, t_button *b);

void			ft_save_main(t_doom *d);
void			ft_save(t_doom *d, t_button *b);
void			ft_save_as(t_doom *d);

void			ft_ctrz_main(t_doom *d);
void			ft_ctrz(t_doom *d, t_button *b);
void			ft_ctrl_z(t_doom *d);

void			ft_textures_main(t_doom *d);
void			ft_put_tex(t_doom *d, t_button *tmp, t_tex *t, int flag);
void			ft_make_black(t_doom *d, t_button *tmp, t_tex *tex, int flag);


void			ft_textures_left_main(t_doom *d);
void			ft_left(t_doom *d, t_button *b);

void			ft_textures_right_main(t_doom *d);
void			ft_right(t_doom *d, t_button *b);
void			ft_scroll_left(t_doom *d);
void			ft_scroll_right(t_doom *d);


void			ft_choose_tex(t_doom *f, int x, int y);
void			ft_tex_arrow(t_doom *d, int x, int i);





void			ft_valid_arg(int argc, char **argv, t_doom *d);

void			ft_create_nodes(t_line **head, t_doom *d);

void			ft_free_lst(t_doom *f, int flag);


/* secondary functions */
int				ft_lst_len(t_line *ln);
void			ft_print_lst(t_doom *f);

//functions to work with SDL
void			ft_surface_set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void			ft_clear_surface(t_doom *d);
Uint32			ft_surface_get_pixel(SDL_Surface *surface, int x, int y);


#endif
