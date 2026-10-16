#include "shared/const.h"
#include "shared/enum.h"
#include "shared/struct.h"
#include "source/runtime.h"
#include "source/common.h"

int hook_press(int keycode, t_game *g)
{
	int kid;

	if (keycode == K_ESC)
		return runtime_close(g);
	// implement K_W for changing map view if required
	kid = common_kid(keycode);
	if (kid >= 0 && kid < GK_COUNT)
		g->keys[kid] = 1;
	return (0);
}

int hook_release(int keycode, t_game *g)
{
	int kid;

	kid = common_kid(keycode);
	if (kid >= 0 && kid < GK_COUNT)
		g->keys[kid] = 0;
	return (0);
}

int hook_out(t_game *g)
{
	int i;

	i = 0;
	while (i < GK_COUNT)
		g->keys[i++] = 0;
	return (0);
}
