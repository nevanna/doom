#include "engine.h"

void	ft_draw_screen(t_engine *engine)
{
	int	renderedsectors[engine->num_sectors];
	t_item queue[32];

	engine->tmp.max_queue = 32;
	engine->tmp.head = queue;
	engine->tmp.tail = queue;
	ft_bzero(engine->tmp.ytop, sizeof(engine->tmp.ytop));
	for (unsigned x = 0; x < W; ++x)
		engine->tmp.ybottom[x] = H - 1;
	for (unsigned n = 0; n < engine->num_sectors; ++n)
		renderedsectors[n] = 0;
	/* Begin whole-screen rendering from where the player is. */
	engine->tmp.head[0].sectorno = engine->player.sector;
	engine->tmp.head[0].sx1 = 0;
	engine->tmp.head[0].sx2 = W - 1;

	if (++engine->tmp.head == queue + engine->tmp.max_queue)
		engine->tmp.head = queue;
	while (engine->tmp.head != engine->tmp.tail)
	{
		/* Pick a sector & slice from the queue to draw */
		engine->tmp.now = *engine->tmp.tail;
		if (++engine->tmp.tail == queue + engine->tmp.max_queue)
			engine->tmp.tail = queue;
		if (renderedsectors[engine->tmp.now.sectorno] & 0x21)
			continue; // Odd = still rendering, 0x20 = give up
		++renderedsectors[engine->tmp.now.sectorno];
		engine->tmp.sect = &engine->sectors[engine->tmp.now.sectorno];
		/* Render each wall of this sector that is facing towards player. */
		ft_render_walls(engine, queue);
		// for s in sector's edges
		++renderedsectors[engine->tmp.now.sectorno];
	} // render any other queued sectors
}
