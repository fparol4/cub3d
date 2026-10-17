#include "source/hook.h"
#include "core/const.h"
#include "source/runtime.h"
#include "source/common.h"

int hook_press(int keycode, t_game *g)
{
	int kid;

	if (keycode == K_ESC)
		return runtime_close(g);
	kid = common_kid(keycode);
	if (kid == GK_VIEW)
	{
		if (g->keys[GK_VIEW])
			return (0);
		g->keys[GK_VIEW] = 1;
		if (g->view == V_3D)
			g->view = V_MINIMAP;
		else
			g->view = V_3D;
		return (0);
	}
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
