#ifndef HOOK_H
#define HOOK_H
	#include "shared/cub3d.h"

	int hook_out(t_game *g);
	int hook_press(int keycode, t_game *g);
	int hook_release(int keycode, t_game *g);
#endif
