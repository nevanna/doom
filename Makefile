NAME = doom

FILES =	ft_atof.c \
		ft_col_dec.c \
		ft_draw_ask_about_host.c \
		ft_draw_func.c \
		ft_draw_game.c \
		ft_draw_intro.c \
		ft_draw_net_game.c \
		ft_draw_net_part.c \
		ft_draw_screen.c \
		ft_draw_something_er.c \
		ft_key.c \
		ft_key_handle.c \
		ft_keys_manager.c \
		ft_main_loop.c \
		ft_manage_data.c \
		ft_menu.c \
		ft_menu_net.c \
		ft_menu_spec.c \
		ft_mouse.c \
		ft_music.c \
		ft_net_client.c \
		ft_net_server.c \
		ft_network.c \
		ft_new_load_data.c \
		ft_portal_animation.c \
		ft_realloc.c \
		ft_render.c \
		ft_render_walls.c \
		ft_scan_digit.c \
		ft_secondary.c \
		ft_sect_data.c \
		ft_start_game.c \
		ft_sup_functions.c \
		ft_tab_menu.c \
		ft_times_of_day.c \
		ft_unload_data.c \
		ft_vert_data.c \
		ft_work_with_data.c \
		ft_work_with_text.c \
		ft_demons.c\
		main.c

SRC = $(addprefix src/, $(FILES))

OBJ = $(addprefix objectives/, $(FILES:.c=.o))
HEADERS = -I include -I libft/include  -I libft/src
ADD = -framework OpenGL -framework AppKit
INCL = -I.  -I src/ \
		-I SDL2/SDL2.framework/Versions/A/Headers \
		-I SDL2/SDL2_image.framework/Versions/A/Header \
		-I SDL2/SDL2_net.framework/Versions/A/Header \
		-I SDL2/SDL2_ttf.framework/Versions/A/Header \
		-I SDL2/SDL2_mixer.framework/Versions/A/Header \
		-F SDL2/
FRAMEWORK = -F SDL2 -framework SDL2\
			-F SDL2 -framework SDL2_image\
			-F SDL2 -framework SDL2_net\
			-F SDL2 -framework SDL2_mixer\
			-F SDL2 -framework SDL2_ttf

FLAGS = -Wall -Wextra -Werror

LFT = -L libft/src -lft

#game: $(NAME)

all: compile | $(NAME)

compile:
	make -C ./libft/src
	make -C ./editor

$(NAME): $(OBJ)
	#$(FLAGS) -fsanitize=address -g
	gcc -o $(NAME) $(SRC) $(INCL) $(LFT) $(ADD) $(FRAMEWORK) $(HEADERS)
	@install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2 SDL2/SDL2.framework/Versions/A/SDL2 $(NAME)
	@install_name_tool -change @rpath/SDL2_mixer.framework/Versions/A/SDL2_mixer SDL2/SDL2_mixer.framework/Versions/A/SDL2_mixer $(NAME)
	@install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL2_image SDL2/SDL2_image.framework/Versions/A/SDL2_image $(NAME)
	@install_name_tool -change @rpath/SDL2_net.framework/Versions/A/SDL2_net SDL2/SDL2_net.framework/Versions/A/SDL2_net $(NAME)
	@install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL2_ttf SDL2/SDL2_ttf.framework/Versions/A/SDL2_ttf $(NAME)

objectives/%.o: src/%.c | objectives
	gcc  $(INCL) $(HEADERS) -o $@ -c $<


clean:
	rm -rf objectives/
	make clean -C ./libft/src
	make clean -C ./editor

fclean: clean
	rm -Rf $(NAME)
	make fclean -C ./libft/src
	make fclean -C ./editor



objectives:
	mkdir objectives/

re: fclean all
