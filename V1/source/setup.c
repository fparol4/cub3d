#include "shared/cub3d.h"
#include "source/runtime.h"
#include "source/hook.h"

int setup_mlx(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (1);
	g->win = mlx_new_window(g->mlx, S_WIDTH, S_HEIGHT, G_NAME);
	if (!g->win)
		return (1);
	return (0);
}

int setup_hook(t_game *g)
{
	mlx_hook(g->win, EVT_KEY_PRESS, 0, hook_press, g);
	mlx_hook(g->win, EVT_KEY_RELEASE, 0, hook_release, g);
	mlx_hook(g->win, EVT_FOCUS_OUT, 0, hook_out, g);
	mlx_hook(g->win, EVT_EXIT, 0, runtime_close, g);
	mlx_loop_hook(g->mlx, screen_frame, g);
	return (0);
}

int setup_frame(t_game *g)
{
	g->frame.width = S_WIDTH;
	g->frame.height = S_HEIGHT;
	g->frame.ptr = mlx_new_image(
		g->mlx,
		g->frame.width,
		g->frame.height
	);
	if (!g->frame.ptr)
		return (1);
	g->frame.addr = mlx_get_data_addr(
		g->frame.ptr,
		&g->frame.bpp,
		&g->frame.llen,
		&g->frame.endian
	);
	if (!g->frame.addr)
		return (1);
	ft_memset(g->frame.addr, 0, g->frame.llen * g->frame.height);
	return (0);
}

int setup_game(t_game *g)
{
	if (setup_mlx(g))
		return (runtime_cleanup(g), 1);
	if (setup_hook(g))
		return (runtime_cleanup(g), 1);
	if (setup_frame(g))
		return (runtime_cleanup(g), 1);
	return (0);
}
