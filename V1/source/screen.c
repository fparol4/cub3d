
#include "source/screen.h"
#include "source/render.h"
#include "libft.h"
#include "shared/enum.h"

int screen_clear(t_game *g)
{
	if (!g || !g->frame.addr)
		return (1);
	ft_memset(g->frame.addr, 0, g->frame.llen * g->frame.height);
	return (0);
}

int screen_render(t_game *g)
{
	if (g->view == V_3D)
		render_3D(g);
	else
		render_2D(g);
	mlx_put_image_to_window(g->mlx, g->win, g->frame.ptr, 0, 0);
	return (0);
}
