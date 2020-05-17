#include "engine.h"

void	ft_player_input(t_engine *engine)
{
	if (engine->sdl.event.key.keysym.sym == 'w')
		engine->player.wsad[0] = engine->sdl.event.type == SDL_KEYDOWN;
	else if (engine->sdl.event.key.keysym.sym == 's')
		engine->player.wsad[1] = engine->sdl.event.type == SDL_KEYDOWN;
	else if (engine->sdl.event.key.keysym.sym == 'a')
		engine->player.wsad[2] = engine->sdl.event.type == SDL_KEYDOWN;
	else if (engine->sdl.event.key.keysym.sym == 'd')
		engine->player.wsad[3] = engine->sdl.event.type == SDL_KEYDOWN;
	else if (engine->sdl.event.key.keysym.sym == ' ')
	{
		if (engine->player.ground)
		{
			engine->player.velocity.z += 0.5;
			engine->player.falling = 1;
		}
	}
	else if (engine->sdl.event.key.keysym.sym == SDLK_RCTRL ||
		engine->sdl.event.key.keysym.sym == SDLK_LCTRL)
	{
		engine->player.ducking = engine->sdl.event.type == SDL_KEYDOWN;
		engine->player.falling = 1;
	}
}

void	ft_mouse(t_engine *engine)
{
	int	x;
	int	y;

	// SDL_ShowCursor(SDL_ENABLE);
	// //SDL_ShowCursor(SDL_DISABLE);
	SDL_GetRelativeMouseState(&x, &y);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	engine->player.angle += x * 0.03f;
	engine->player.temp_yaw = clamp(engine->player.temp_yaw + y * 0.05f, -5, 5);
	engine->player.yaw = engine->player.temp_yaw - engine->player.velocity.z * 0.5f;
	ft_move_player(0, 0, engine);
	ft_bzero(engine->player.move_vec, sizeof(engine->player.move_vec));
	if (engine->player.wsad[0])
	{
		engine->player.move_vec[0] += engine->player.anglecos * 0.2f;
		engine->player.move_vec[1] += engine->player.anglesin * 0.2f;
	}
	if (engine->player.wsad[1])
	{
		engine->player.move_vec[0] -= engine->player.anglecos * 0.2f;
		engine->player.move_vec[1] -= engine->player.anglesin * 0.2f;
	}
	if (engine->player.wsad[2])
	{
		engine->player.move_vec[0] += engine->player.anglesin * 0.2f;
		engine->player.move_vec[1] -= engine->player.anglecos * 0.2f;
	}
	if (engine->player.wsad[3])
	{
		engine->player.move_vec[0] -= engine->player.anglesin * 0.2f;
		engine->player.move_vec[1] += engine->player.anglecos * 0.2f;
	}
	engine->player.pushing = engine->player.wsad[0] || engine->player.wsad[1] || engine->player.wsad[2] || engine->player.wsad[3];
	engine->player.acceleration = engine->player.pushing ? 0.4 : 0.2;
	engine->player.velocity.x = engine->player.velocity.x * (1 - engine->player.acceleration) + engine->player.move_vec[0] * engine->player.acceleration;
	engine->player.velocity.y = engine->player.velocity.y * (1 - engine->player.acceleration) + engine->player.move_vec[1] * engine->player.acceleration;
	if (engine->player.pushing)
		engine->player.moving = 1;
}

