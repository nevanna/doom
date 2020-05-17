#include "engine.h"

static void vline(int x, int y1, int y2, int top, int middle, int bottom, t_engine *engine)
{
	int *pix = (int *)engine->sdl.sur_ptr->pixels;
	y1 = clamp(y1, 0, H - 1);
	y2 = clamp(y2, 0, H - 1);
	if (y2 == y1)
		pix[y1 * W + x] = middle;
	else if (y2 > y1)
	{
		pix[y1 * W + x] = top;
		for (int y = y1 + 1; y < y2; ++y)
			pix[y * W + x] = middle;
		pix[y2 * W + x] = bottom;
	}
}

static void ft_project_ceiling_floor(t_engine *engine)
{
	engine->tmp.y1a = H / 2 - (int)(Yaw(engine->tmp.yceil,
										engine->tmp.tz1) * engine->tmp.yscale1);
	engine->tmp.y1b = H / 2 - (int)(Yaw(engine->tmp.yfloor,
										engine->tmp.tz1) * engine->tmp.yscale1);
	engine->tmp.y2a = H / 2 - (int)(Yaw(engine->tmp.yceil,
										engine->tmp.tz2) * engine->tmp.yscale2);
	engine->tmp.y2b = H / 2 - (int)(Yaw(engine->tmp.yfloor,
										engine->tmp.tz2) * engine->tmp.yscale2);
	/* The same for the neighboring sector */
	engine->tmp.ny1a = H / 2 - (int)(Yaw(engine->tmp.nyceil,
										engine->tmp.tz1) * engine->tmp.yscale1);
	engine->tmp.ny1b = H / 2 - (int)(Yaw(engine->tmp.nyfloor,
										engine->tmp.tz1) * engine->tmp.yscale1);
	engine->tmp.ny2a = H / 2 - (int)(Yaw(engine->tmp.nyceil,
										engine->tmp.tz2) * engine->tmp.yscale2);
	engine->tmp.ny2b = H / 2 - (int)(Yaw(engine->tmp.nyfloor,
										engine->tmp.tz2) * engine->tmp.yscale2);
}

static void ft_sector_behind(t_engine *engine, int x)
{
	if (engine->tmp.neighbor >= 0)
	{
		/* Same for _their_ floor and ceiling */
		engine->tmp.nya = (x - engine->tmp.x1) * (engine->tmp.ny2a -
		engine->tmp.ny1a) / (engine->tmp.x2 - engine->tmp.x1) +
		engine->tmp.ny1a, engine->tmp.cnya =
		clamp(engine->tmp.nya, engine->tmp.ytop[x], engine->tmp.ybottom[x]);
		engine->tmp.nyb = (x - engine->tmp.x1) * (engine->tmp.ny2b -
		engine->tmp.ny1b) / (engine->tmp.x2 - engine->tmp.x1) +
		engine->tmp.ny1b, engine->tmp.cnyb =
		clamp(engine->tmp.nyb, engine->tmp.ytop[x], engine->tmp.ybottom[x]);
		/* If our ceiling is higher than their ceiling, render upper wall */
		engine->tmp.r1 = 0x010101 * (255 - engine->tmp.z);
		engine->tmp.r2 = 0x040007 * (31 - engine->tmp.z / 8);
		vline(x, engine->tmp.cya, engine->tmp.cnya - 1, 0, x == engine->tmp.x1 ||
				x == engine->tmp.x2 ? 0 : engine->tmp.r1, 0, engine); // Between our and their ceiling
		engine->tmp.ytop[x] = clamp(max(engine->tmp.cya,
								engine->tmp.cnya), engine->tmp.ytop[x], H - 1);								// Shrink the remaining window below these ceilings
		/* If our floor is lower than their floor, render bottom wall */
		vline(x, engine->tmp.cnyb + 1, engine->tmp.cyb, 0, x == engine->tmp.x1 ||
				x == engine->tmp.x2 ? 0 : engine->tmp.r2, 0, engine); // Between their and our floor
		engine->tmp.ybottom[x] = clamp(min(engine->tmp.cyb,
								engine->tmp.cnyb), 0, engine->tmp.ybottom[x]);								// Shrink the remaining window above these floors
	}
	else
	{
		/* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
		engine->tmp.r = 0x010101 * (255 - engine->tmp.z);
		vline(x, engine->tmp.cya, engine->tmp.cyb, 0, x == engine->tmp.x1 ||
				x == engine->tmp.x2 ? 0 : engine->tmp.r, 0, engine);
	}
}

static void ft_render_wall(t_engine *engine)
{
	engine->tmp.beginx = max(engine->tmp.x1, engine->tmp.now.sx1);
	engine->tmp.endx = min(engine->tmp.x2, engine->tmp.now.sx2);
	for (int x = engine->tmp.beginx; x <= engine->tmp.endx; ++x)
	{
		/* Calculate the Z coordinate for this point. (Only used for lighting.) */
		engine->tmp.z = ((x - engine->tmp.x1) *
						(engine->tmp.tz2 - engine->tmp.tz1) /
						(engine->tmp.x2 - engine->tmp.x1) + engine->tmp.tz1) * 8;
		/* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
		engine->tmp.ya = (x - engine->tmp.x1) *
						(engine->tmp.y2a - engine->tmp.y1a) /
						(engine->tmp.x2 - engine->tmp.x1) + engine->tmp.y1a;
		engine->tmp.cya = clamp(engine->tmp.ya,
						engine->tmp.ytop[x], engine->tmp.ybottom[x]); // top
		engine->tmp.yb = (x - engine->tmp.x1) *
						(engine->tmp.y2b - engine->tmp.y1b) /
						(engine->tmp.x2 - engine->tmp.x1) + engine->tmp.y1b;
		engine->tmp.cyb = clamp(engine->tmp.yb,
						engine->tmp.ytop[x], engine->tmp.ybottom[x]); // bottom

		/* Render ceiling: everything above this sector's ceiling height. */
		vline(x, engine->tmp.ytop[x], engine->tmp.cya - 1,
						0x111111, 0x222222, 0x111111, engine);
		/* Render floor: everything below this sector's floor height. */
		vline(x, engine->tmp.cyb + 1, engine->tmp.ybottom[x],
						0x0000FF, 0x0000AA, 0x0000FF, engine);

		/* Is there another sector behind this edge? */
		ft_sector_behind(engine, x);
	}
}

void ft_render_walls(t_engine *engine, t_item *queue)
{
	for (unsigned s = 0; s < engine->tmp.sect->npoints; ++s)
	{
		/* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
		ft_acquire_coords(engine, s);
		/* Rotate them around the player's view */
		ft_rotate_coords(engine);
		/* Is the wall at least partially in front of the player? */
		if (engine->tmp.tz1 <= 0 && engine->tmp.tz2 <= 0)
			continue;
		/* If it's partially behind the player, clip it against player's view frustrum */
		ft_check_wall_behind(engine);
		/* Do perspective transformation */
		ft_perspective_transform(engine);
		if (engine->tmp.x1 >= engine->tmp.x2 ||
			engine->tmp.x2 < engine->tmp.now.sx1 ||
			engine->tmp.x1 > engine->tmp.now.sx2)
			continue; // Only render if it's visible
		/* Acquire the floor and ceiling heights, relative to where the player's view is */
		engine->tmp.yceil = engine->tmp.sect->ceil - engine->player.where.z;
		engine->tmp.yfloor = engine->tmp.sect->floor - engine->player.where.z;
		/* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
		engine->tmp.neighbor = engine->tmp.sect->neighbors[s];
		if (engine->tmp.neighbor >= 0) // Is another sector showing through this portal?
		{
			engine->tmp.nyceil = engine->sectors[engine->tmp.neighbor].ceil -
													engine->player.where.z;
			engine->tmp.nyfloor = engine->sectors[engine->tmp.neighbor].floor -
													engine->player.where.z;
		}
		/* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
		ft_project_ceiling_floor(engine);
		/* Render the wall. */
		ft_render_wall(engine);
		/* Schedule the neighboring sector for rendering within the window formed by this wall. */
		ft_schedule_neighbor_sect(engine, queue);
	}
}
