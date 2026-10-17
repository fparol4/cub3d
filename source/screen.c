/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "libft.h"
#include "mlx.h"
#include "source/render.h"
#include "source/screen.h"
#include <math.h>

static t_viewport	_screen_overlay(t_game *g)
{
	t_viewport	v;
	double		scale;

	scale = fmin((double)MINIMAP_MAX_WIDTH / g->map.width,
			(double)MINIMAP_MAX_HEIGHT / g->map.height);
	v.width = (int)(g->map.width * scale);
	v.height = (int)(g->map.height * scale);
	if (v.width < 1)
		v.width = 1;
	if (v.height < 1)
		v.height = 1;
	v.x = g->frame.width - v.width - MINIMAP_MARGIN;
	v.y = MINIMAP_MARGIN;
	return (v);
}

void	screen_toggle_view(t_game *g)
{
	if (g->view == V_3D)
		g->view = V_MINIMAP;
	else
		g->view = V_3D;
	g->dirty = 1;
}

int	screen_clear(t_game *g)
{
	if (!g || !g->frame.addr)
		return (1);
	ft_memset(g->frame.addr, 0, g->frame.llen * g->frame.height);
	return (0);
}

int	screen_render(t_game *g)
{
	t_viewport	v;

	if (!g || !g->frame.addr || !g->win)
		return (1);
	if (g->view == V_3D)
	{
		render_3d(g);
		v = _screen_overlay(g);
	}
	else
		v = (t_viewport){0, 0, g->frame.width, g->frame.height};
	render_map_view(g, &v);
	mlx_put_image_to_window(g->mlx, g->win, g->frame.ptr, 0, 0);
	return (0);
}
