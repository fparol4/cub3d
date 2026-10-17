#include "shared/const.h"
#include "shared/struct.h"
#include "source/common.h"
#include "source/screen.h"
#include "source/player.h"

static void _game_capframe(double *elapsed)
{
	if (*elapsed < 0.0)
		*elapsed = 0;
	if (*elapsed > PLAYER_MAX_ELAPSED)
		*elapsed = PLAYER_MAX_ELAPSED;
}

int game_frame(t_game *g)
{
	double now;
	double elapsed;

	now = common_now();
	if (g->last_frame == 0.0)
		elapsed = 0.0;
	else
		elapsed = now - g->last_frame;
	g->last_frame = now;
	_game_capframe(&elapsed);
	player_update(g, elapsed);
	screen_clear(g);
	screen_render(g);
	return (0);
}
