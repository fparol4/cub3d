#include "shared/const.h"
#include "shared/enum.h"
#include "shared/struct.h"
#include "source/runtime.h"

int key_id(int keycode)
{
	if (keycode == K_A)
		return (GK_LEFT);
	if (keycode == K_D)
		return (GK_RIGHT);
	if (keycode == K_LEFT)
		return (GK_ACLOCKWISE);
	if (keycode == K_RIGHT)
		return (GK_CLOCKWISE);
	if (keycode == K_M)
		return (GK_VIEW);
	if (keycode == K_W || keycode == K_UP)
		return (GK_FORWARD);
	if (keycode == K_S || keycode == K_DOWN)
		return (GK_BACKWARD);
	return (-1);
}

int hook_close(t_game *g)
{
	return runtime_close(g);
}

int hook_press(int keycode, t_game *g)
{
	int kid;

	if (keycode == K_ESC)
		return runtime_close(g);
	/* if (kc == K_M)
	 	return hook_mapview(g); */
	kid = key_id(keycode);
	if (kid >= 0 && kid < GK_COUNT)
		g->keys[kid] = 1;
	return (0);
}

int hook_release(int keycode, t_game *g)
{
	int kid;

	kid = key_id(keycode);
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

int hook_frame(t_game *g)
{
	return game_frame(g);
}
