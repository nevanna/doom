#include "engine.h"

static void ft_vert_col_det(t_engine *engine)
{
	engine->player.eyeheight = engine->player.ducking ? DuckHeight : EyeHeight;
	engine->player.ground = !engine->player.falling;
	if (engine->player.falling)
	{
		engine->player.velocity.z -= 0.05f;
		float nextz = engine->player.where.z + engine->player.velocity.z;
		if (engine->player.velocity.z < 0 && nextz < engine->sectors[engine->player.sector].floor + engine->player.eyeheight)
		{
			engine->player.where.z = engine->sectors[engine->player.sector].floor + engine->player.eyeheight;
			engine->player.velocity.z = 0;
			engine->player.falling = 0;
			engine->player.ground = 1;
		}
		else if (engine->player.velocity.z > 0 && nextz > engine->sectors[engine->player.sector].ceil)
		{
			engine->player.velocity.z = 0;
			engine->player.falling = 1;
		}
		if (engine->player.falling)
		{
			engine->player.where.z += engine->player.velocity.z;
			engine->player.moving = 1;
		}
	}
}

static void ft_horiz_col_det(t_engine *engine)
{
	if (engine->player.moving)
	{
		float px = engine->player.where.x, py = engine->player.where.y;
		float dx = engine->player.velocity.x, dy = engine->player.velocity.y;

		const t_sector *const sect = &engine->sectors[engine->player.sector];
		const t_xy *const vert = sect->vertex;
		for (unsigned s = 0; s < sect->npoints; ++s)
			if (IntersectBox(px, py, px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) &&
				PointSide(px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) < 0)
			{
				float hole_low = sect->neighbors[s] < 0 ? 9e9 : max(sect->floor, engine->sectors[sect->neighbors[s]].floor);
				float hole_high = sect->neighbors[s] < 0 ? -9e9 : min(sect->ceil, engine->sectors[sect->neighbors[s]].ceil);
				if (hole_high < engine->player.where.z + HeadMargin || hole_low > engine->player.where.z - engine->player.eyeheight + KneeHeight)
				{
					float xd = vert[s + 1].x - vert[s + 0].x, yd = vert[s + 1].y - vert[s + 0].y;
					dx = xd * (dx * xd + yd * dy) / (xd * xd + yd * yd);
					dy = yd * (dx * xd + yd * dy) / (xd * xd + yd * yd);
					engine->player.moving = 0;
				}
			}
		ft_move_player(dx, dy, engine);
		engine->player.falling = 1;
	}
}

double angle_point (t_xy a, t_xy b, t_xy c)
{
   double x1 = a.x - b.x, x2 = c.x - b.x;
   double y1 = a.y - b.y, y2 = c.y - b.y;
   double d1 = sqrt (x1 * x1 + y1 * y1);
   double d2 = sqrt (x2 * x2 + y2 * y2);
   return (acos((x1 * x2 + y1 * y2) / (d1 * d2)));
}

void	ft_angle_col_det(t_engine *engine)
{
	unsigned s = 0;
	float angle = 0;
	float px = engine->player.where.x, py = engine->player.where.y;
	const t_sector *const sect = &engine->sectors[engine->player.sector];
	const t_xy *const vert = sect->vertex;
	float min = sqrt(pow(vert[0].x - px, 2) + pow(vert[0].y - py, 2));
	unsigned temp = 0;
	while (s < sect->npoints)
	{
		if (sqrt(pow(vert[s].x - px, 2) + pow(vert[s].y - py, 2)) < min)
		{
			min = sqrt(pow(vert[s].x - px, 2) + pow(vert[s].y - py, 2));
			temp = s;
		}
		s++;
	}
	if (min < 1)
	{
		if (temp == 0)
			angle = angle_point(vert[temp], vert[temp + 1], vert[temp + 2]);
		else if (temp == sect->npoints)
			angle = angle_point(vert[temp], vert[temp - 1], vert[temp - 2]);
		else
			angle = angle_point(vert[temp], vert[temp + 1], vert[temp + 2]);
	}
	if (angle < 1)
		printf("angle < 1");
	if (angle > 1)
		printf("angle > 1");
	if (angle == 0)
		printf ("angle = 0");
}

void	ft_col_det(t_engine *engine)
{
	ft_vert_col_det(engine);
	ft_horiz_col_det(engine);
	//ft_angle_col_det(engine);
}
