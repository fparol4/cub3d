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

#include "core/cub3d.h"

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
	v.x = g->graphics.frame.width - v.width - MINIMAP_MARGIN;
	v.y = MINIMAP_MARGIN;
	return (v);
}

void	screen_toggle_view(t_game *g)
{
	if (g->state.view == V_3D)
		g->state.view = V_MINIMAP;
	else
		g->state.view = V_3D;
	g->state.needs_redraw = 1;
}

int	screen_clear(t_game *g)
{
	if (!g || !g->graphics.frame.addr)
		return (1);
	ft_memset(g->graphics.frame.addr, 0, g->graphics.frame.stride * g->graphics.frame.height);
	return (0);
}

int	screen_render(t_game *g)
{
	t_viewport	v;

	if (!g || !g->graphics.frame.addr || !g->graphics.window)
		return (1);
	if (g->state.view == V_3D)
	{
		render_3d(g);
		v = _screen_overlay(g);
	}
	else
		v = (t_viewport){0, 0, g->graphics.frame.width, g->graphics.frame.height};
	render_map_view(g, &v);
	mlx_put_image_to_window(g->graphics.mlx, g->graphics.window, g->graphics.frame.ptr, 0, 0);
	return (0);
}
