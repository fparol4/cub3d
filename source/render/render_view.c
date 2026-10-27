/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_view.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:19 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_render_tile_color(char tile)
{
	if (tile == T_WALL)
		return (COLOR_WALL);
	if (tile == T_FLOOR)
		return (COLOR_FLOOR);
	return (COLOR_VOID);
}

static void	_render_map_cell(t_game *g, t_viewport *v, t_point cell)
{
	t_point	from;
	t_point	to;

	from = render_map_point(g, v, cell.x, cell.y);
	to = render_map_point(g, v, cell.x + 1, cell.y + 1);
	if (to.x - from.x > 2 && to.y - from.y > 2)
	{
		render_rect(&g->graphics.frame, from, to, COLOR_TILE_BORDER);
		from.x++;
		from.y++;
		to.x--;
		to.y--;
	}
	render_rect(&g->graphics.frame, from, to,
		_render_tile_color(g->map.grid[cell.y][cell.x]));
}

static void	_render_map(t_game *g, t_viewport *v)
{
	t_point	cell;

	cell.y = 0;
	while (cell.y < g->map.height)
	{
		cell.x = 0;
		while (cell.x < g->map.width)
		{
			_render_map_cell(g, v, cell);
			cell.x++;
		}
		cell.y++;
	}
}

static void	_render_player(t_game *g, t_viewport *v)
{
	t_point	center;
	t_point	from;
	t_point	to;
	int		size;

	center = render_map_point(g, v, g->player.pos_x, g->player.pos_y);
	size = v->width / g->map.width;
	if (v->height / g->map.height < size)
		size = v->height / g->map.height;
	size /= 4;
	if (size < 2)
		size = 2;
	from = (t_point){center.x - size / 2, center.y - size / 2};
	to = (t_point){from.x + size, from.y + size};
	render_rect(&g->graphics.frame, from, to, COLOR_PLAYER);
}

void	render_map_view(t_game *g, t_viewport *v)
{
	_render_map(g, v);
	render_fov(g, v);
	_render_player(g, v);
}
