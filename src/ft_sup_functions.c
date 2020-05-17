#include "engine.h"

void		ft_acquire_coords(t_engine *engine, unsigned s)
{
	engine->tmp.vx1 = engine->tmp.sect->vertex[s + 0].x - engine->player.where.x;
	engine->tmp.vy1 = engine->tmp.sect->vertex[s + 0].y - engine->player.where.y;
	engine->tmp.vx2 = engine->tmp.sect->vertex[s + 1].x - engine->player.where.x;
	engine->tmp.vy2 = engine->tmp.sect->vertex[s + 1].y - engine->player.where.y;
}

void		ft_rotate_coords(t_engine *engine)
{
	engine->tmp.pcos = engine->player.anglecos;
	engine->tmp.psin = engine->player.anglesin;
	engine->tmp.tx1 = engine->tmp.vx1 * engine->tmp.psin -
					engine->tmp.vy1 * engine->tmp.pcos;
	engine->tmp.tz1 = engine->tmp.vx1 * engine->tmp.pcos +
					engine->tmp.vy1 * engine->tmp.psin;
	engine->tmp.tx2 = engine->tmp.vx2 * engine->tmp.psin -
					engine->tmp.vy2 * engine->tmp.pcos;
	engine->tmp.tz2 = engine->tmp.vx2 * engine->tmp.pcos +
					engine->tmp.vy2 * engine->tmp.psin;
}

static void	ft_check(t_engine *engine)
{
	if (engine->tmp.tz1 < engine->tmp.nearz)
	{
		if (engine->tmp.i1.y > 0)
		{
			engine->tmp.tx1 = engine->tmp.i1.x;
			engine->tmp.tz1 = engine->tmp.i1.y;
		}
		else
		{
			engine->tmp.tx1 = engine->tmp.i2.x;
			engine->tmp.tz1 = engine->tmp.i2.y;
		}
	}
	if (engine->tmp.tz2 < engine->tmp.nearz)
	{
		if (engine->tmp.i1.y > 0)
		{
			engine->tmp.tx2 = engine->tmp.i1.x;
			engine->tmp.tz2 = engine->tmp.i1.y;
		}
		else
		{
			engine->tmp.tx2 = engine->tmp.i2.x;
			engine->tmp.tz2 = engine->tmp.i2.y;
		}
	}
}

void	ft_check_wall_behind(t_engine *engine)
{
	if (engine->tmp.tz1 <= 0 || engine->tmp.tz2 <= 0)
	{
		engine->tmp.nearz = 1e-4f;
		engine->tmp.farz = 5;
		engine->tmp.nearside = 1e-5f;
		engine->tmp.farside = 20.f;
		// Find an intersection between the wall and the approximate edges of player's view
		engine->tmp.i1 = Intersect(engine->tmp.tx1, engine->tmp.tz1,
								   engine->tmp.tx2, engine->tmp.tz2,
								   -engine->tmp.nearside, engine->tmp.nearz,
								   -engine->tmp.farside, engine->tmp.farz);
		engine->tmp.i2 = Intersect(engine->tmp.tx1, engine->tmp.tz1,
								   engine->tmp.tx2, engine->tmp.tz2,
								   engine->tmp.nearside, engine->tmp.nearz,
								   engine->tmp.farside, engine->tmp.farz);
		ft_check(engine);
	}
}

void	ft_perspective_transform(t_engine *engine)
{
	engine->tmp.xscale1 = hfov / engine->tmp.tz1;
	engine->tmp.yscale1 = vfov / engine->tmp.tz1;
	engine->tmp.x1 = W / 2 - (int)(engine->tmp.tx1 * engine->tmp.xscale1);
	engine->tmp.xscale2 = hfov / engine->tmp.tz2;
	engine->tmp.yscale2 = vfov / engine->tmp.tz2;
	engine->tmp.x2 = W / 2 - (int)(engine->tmp.tx2 * engine->tmp.xscale2);
}

void	ft_schedule_neighbor_sect(t_engine *engine, t_item *queue)
{
	if (engine->tmp.neighbor >= 0 && engine->tmp.endx >= engine->tmp.beginx &&
		(engine->tmp.head + engine->tmp.max_queue + 1 - engine->tmp.tail) % engine->tmp.max_queue)
	{
		engine->tmp.head[0].sectorno = engine->tmp.neighbor;
		engine->tmp.head[0].sx1 = engine->tmp.beginx;
		engine->tmp.head[0].sx2 = engine->tmp.endx;
		if (++engine->tmp.head == queue + engine->tmp.max_queue)
			engine->tmp.head = queue;
	}
}
