#ifndef ENGINE_H
# define ENGINE_H
# include <math.h>
# include <stdio.h>
# include <time.h>
# include <SDL2/SDL.h>
# include	"libft.h"
/* mac os */
# include <SDL2_image/SDL_image.h>
# include <SDL2_net/SDL_net.h>
# include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

/* linux
# include "../../../usr/include/SDL2/SDL_image.h"
# include "../../../usr/include/SDL2/SDL_net.h"
# include "../../../usr/include/SDL2/SDL_ttf.h"
# include "../../../usr/include/SDL2/SDL_mixer.h"
*/

# define W 1920
# define H 1080

# define EyeHeight 6
# define DuckHeight 2.5
# define HeadMargin 1
# define KneeHeight 2
# define hfov (0.73f * H)
# define vfov (.2f * H)
# define PLAYER engine.player
# define SECTORS engine.sectors
# define min(a, b) (((a) < (b)) ? (a) : (b))
# define max(a, b) (((a) > (b)) ? (a) : (b))
# define clamp(a, mi, ma) min(max(a, mi), ma)
# define vxs(x0, y0, x1, y1) ((x0) * (y1) - (x1) * (y0))
# define Overlap(a0, a1, b0, b1) (min(a0, a1) <= max(b0, b1) && min(b0, b1) <= max(a0, a1))
# define IntersectBox(x0, y0, x1, y1, x2, y2, x3, y3) (Overlap(x0, x1, x2, x3) && Overlap(y0, y1, y2, y3))
# define PointSide(px, py, x0, y0, x1, y1) vxs((x1) - (x0), (y1) - (y0), (px) - (x0), (py) - (y0))
# define Intersect(x1, y1, x2, y2, x3, y3, x4, y4) ((t_xy){                                                                        \
	vxs(vxs(x1, y1, x2, y2), (x1) - (x2), vxs(x3, y3, x4, y4), (x3) - (x4)) / vxs((x1) - (x2), (y1) - (y2), (x3) - (x4), (y3) - (y4)), \
	vxs(vxs(x1, y1, x2, y2), (y1) - (y2), vxs(x3, y3, x4, y4), (y3) - (y4)) / vxs((x1) - (x2), (y1) - (y2), (x3) - (x4), (y3) - (y4))})
# define Yaw(y, z) (y + z * engine->player.yaw)

typedef struct	s_item
{
	int		sectorno;
	int		sx1;
	int		sx2;
}				t_item;

typedef struct	s_xy
{
	float		x;
	float		y;
}				t_xy;

typedef struct	s_sector
{
	float		floor;
	float		ceil;
	t_xy		*vertex;
	signed char *neighbors;
	unsigned	npoints;
	int			*texturs;
}				t_sector;


typedef struct	s_temp
{
	int			max_queue;
	t_item		queue[32];
	t_item		*head;
	t_item		*tail;
	t_item		now;
	int			ytop[W];
	int			ybottom[W];
	t_sector	*sect;
	float		vx1;
	float		vx2;
	float		vy1;
	float		vy2;
	float		pcos;
	float		psin;
	float		tx1;
	float		tx2;
	float		tz1;
	float		tz2;
	float		nearz;
	float		farz;
	float		nearside;
	float		farside;
	t_xy		i1;
	t_xy		i2;
	float		xscale1;
	float		yscale1;
	int			x1;
	float		xscale2;
	float		yscale2;
	int			x2;
	float		yceil;
	float		yfloor;
	int			neighbor;
	float		nyceil;
	float		nyfloor;
	int			y1a;
	int			y1b;
	int			y2a;
	int			y2b;
	int			ny1a;
	int			ny1b;
	int			ny2a;
	int			ny2b;
	int			beginx;
	int			endx;
	int			z;
	int			ya;
	int			cya;
	int			yb;
	int			cyb;
	int			nya;
	int			cnya;
	int			nyb;
	int			cnyb;
	unsigned	r1;
	unsigned	r2;
	unsigned	r;
}				t_temp;





typedef struct	s_anim
{
	int		lim_sp;
	SDL_Surface	*p_ptr;
	int		lim_h;
	int		lim_w;
	int		i;
}				t_anim;

typedef struct	s_xyz
{
	float		x;
	float		y;
	float		z;
}				t_xyz;

typedef	struct	s_player
{
	t_xyz		where;
	t_xyz		velocity;
	float		angle;
	float		anglesin;
	float		anglecos;
	float		temp_yaw;
	float		yaw;
	unsigned	sector;
	int			wsad[4];
	float		move_vec[2];
	int			ground;
	int			falling;
	int			moving;
	int			ducking;
	int			pushing;
	float		acceleration;
	float		eyeheight;

	int			level;
	int			health;
	int			health_max;
	int			ammo;
	int			ammo_max;
}				t_player;



typedef struct	s_music
{
	//The music that will be played
	Mix_Music *gmusic;
	//The sound effects that will be used
	Mix_Chunk *effect;
	/* data */
}				t_music;

/* that struct for transmit and recive*/
typedef struct	s_data
{
	int	x;
	int	y;
}				t_data;

typedef struct	s_text
{
	int			x;
	int			y;
	char		*text;
	int			size;
	int			r;
	int			g;
	int			b;
}				t_text;

typedef struct	s_sdl
{
	SDL_Window	*win_ptr; //SDL_Window	*window;
	SDL_Surface	*sur_ptr; //SDL_Surface	*screen;
	SDL_Surface	*img_ptr;
	SDL_Surface	*but_start_ptr;
	SDL_Event	event;
	SDL_Rect	destin;
	t_text		text;
}				t_sdl;

typedef struct	s_engine
{
	t_sdl		sdl; //замолочить ее
	t_music		mus;

	/* for net */
	char		*host_name;
	int			port;
	IPaddress	ip;
	TCPsocket	client;
	TCPsocket	server;
	t_data		*data_my;
	t_data		*data_another; /*serv o client depends on flag*/
	t_data		*data_tmp;
	/* end of net */

	t_player	player;
	t_sector	*sectors;
	t_temp		tmp;
	unsigned	num_sectors;
	unsigned	close_request;

	/* flags */
	/* i chose char because 1 weight */
	/* mb i create struct of fl */
	/* Y-Yes, N-No, NUll */
	char			menu_fl;
	char			net_fl;
	char			host_fl;
	char			client_fl;
	char			mouse_fl;
	char			net_err_fl;

	int				m_x;
	int				m_y;

}				t_engine;
/*engine func */
// int				ft_init_engine(t_engine *engine);
// void			ft_load_data(t_engine *engine);
// void			ft_start_game(t_engine *engine);
// void			ft_render(t_engine *engine);
// void			ft_draw_screen(t_engine *engine);
// void			ft_move_player(float dx, float dy, t_engine *engine);
// void			ft_col_det(t_engine *engine);
// void			ft_keys_manager(t_engine *engine);
// void			ft_unload_data(t_engine *engine);
// void			ft_clean_up(t_engine *engine);

/* draw func */
void	ft_draw_game(t_engine *e);
void	ft_draw_func(t_engine *e);
void	ft_main_loop(t_engine *e, SDL_Thread	**thread);
void	ft_general_loop(t_engine *e);
void	ft_draw_net_game(t_engine *e);
void	ft_draw_intro(t_engine *e);
/* animation */
void	ft_portal_animation(t_engine *e, t_anim p, t_text tex);
void	ft_demons_one(t_engine *e);
/* secondary func */
void		ft_return(char *str);
int			ft_time ();
void		ft_delay_sec(int sec);
void		ft_surface_set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
Uint32		ft_surface_get_pixel(SDL_Surface *sur, int x, int y);

/* key event */
int				ft_ch_key(t_engine *e);
int				ft_ch_mouse(t_engine *e);
int				ft_exit_x(t_engine *e);
void			ft_close(t_engine *e);
void			ft_close_net(t_engine *e);
/* menu func */
void	ft_menu(t_engine *e);
void	ft_background(t_engine *e);
void	ft_choose_back(t_engine *e);
void	ft_buttons_m(t_engine *e);

void	ft_menu_net(t_engine *e);
void	ft_menu_spec(t_engine *e);
void	ft_tab_menu(t_engine *e);
void	ft_check_tab_zone(t_engine *e);
void	ft_fill_for_g(t_engine *e);

/* net host */
void	ft_net_server(t_engine *e);
int		ft_listening(t_engine *e);
void	ft_draw_waiting_list(t_engine *e);
void	ft_draw_tired(t_engine *e);

int		ft_connect(void *data);
void	ft_loop_net(t_engine *e);

/* net client */
void	ft_handle_key(t_engine *e);
void	ft_ready_host_name(t_engine *e);
void	ft_back_space(t_engine *e);
void	ft_net_client(t_engine *e);
void	ft_draw_ask_about_host(t_engine *e);

/* work with text */
void		ft_text(t_text *t, t_engine *e);
void		ft_fill_text_struct_c(t_text *t, int r, int g, int b);
void		ft_fill_text_struct(t_text *t, int x, int y, int sz);

/* sdl init */
void	ft_init_sdl(t_engine *e);
void	ft_create_struct_another(t_engine *e, int x, int y);
void	ft_create_struct_my(t_engine *e, int x, int y);

/* music */
void	ft_init_mus(t_engine *e);
void	ft_load_music(t_engine *e, char *name_file);
void	ft_load_chunks(t_engine *e, char *name_file);
void	ft_music_stop_start(t_engine *e, char *str);
void	ft_load_img_music_menu(t_engine *e, char *img, char *music);


/* load data */
float			ft_atof(const char *str);
float			ft_scan_digit(char **str);
int				ft_new_load_data(t_engine *engine, char *file);
void			*ft_realloc(void *mem, int size, size_t segment);
int				ft_sect_data(t_engine *engine, t_xy *vert, char *gnl);
int				ft_vert_data(t_xy **vert_main, char *gnl);

/* rendder */
void			ft_draw_screen(t_engine *engine);
void			ft_render_walls(t_engine *engine, t_item *queue);
void			ft_acquire_coords(t_engine *engine, unsigned s);
void			ft_rotate_coords(t_engine *engine);
void			ft_check_wall_behind(t_engine *engine);
void			ft_perspective_transform(t_engine *engine);
void			ft_schedule_neighbor_sect(t_engine *engine, t_item *queue);
void			ft_render(t_engine *e);
void			ft_start_game(t_engine *engine);
void			ft_player_input(t_engine *e);
void			ft_mouse(t_engine *engine);
void			ft_keys_manager(t_engine *engine);
void			ft_col_det(t_engine *engine);
void			ft_move_player(float dx, float dy, t_engine *engine);
void			ft_load_data(t_engine *engine);


/* gui bar */
void	ft_gui(t_engine *e);
void	ft_health_bar(t_engine *e);

#endif
