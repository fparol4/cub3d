/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "source/raycast.h"
#include "source/render.h"
#include "source/texture.h"
#include <math.h>

static void	_render_background(t_game *g, int x)
{
	char	*dst;
	int		y;
	int		color;

	dst = g->frame.addr + x * (g->frame.bpp / 8);
	y = 0;
	while (y < g->frame.height)
	{
		color = g->map.color_floor;
		if (y < g->frame.height / 2)
			color = g->map.color_ceiling;
		*(unsigned int *)(dst + y * g->frame.llen) = (unsigned int)color;
		y++;
	}
}

static void	_render_column_setup(t_game *g, t_raycast *rc, t_column *col)
{
	double	distance;
	double	offset;

	col->texture = texture_face(g, rc);
	distance = fmax(rc->hit.distance, RENDER_MIN_DISTANCE);
	col->height = (int)(g->frame.height / distance);
	col->top = g->frame.height / 2 - col->height / 2;
	col->start = (int)fmax(col->top, 0);
	col->end = (int)fmin(col->top + col->height, g->frame.height);
	offset = rc->hit.x;
	if (rc->hit.side == RAYSIDE_X)
		offset = rc->hit.y;
	offset -= floor(offset);
	col->tex_x = (int)(offset * col->texture->width);
	if ((rc->hit.side == RAYSIDE_X && rc->ray_x > 0)
		|| (rc->hit.side == RAYSIDE_Y && rc->ray_y < 0))
		col->tex_x = col->texture->width - col->tex_x - 1;
}

static void	_render_wall(t_game *g, int x, t_column *col)
{
	char	*dst;
	char	*src;
	int		y;
	int		tex_y;

	dst = g->frame.addr + x * (g->frame.bpp / 8);
	src = col->texture->addr + col->tex_x * (col->texture->bpp / 8);
	y = col->start;
	while (y < col->end)
	{
		tex_y = (int)((double)(y - col->top) * col->texture->height
				/ col->height);
		*(unsigned int *)(dst + y * g->frame.llen) = *(unsigned int *)
			(src + tex_y * col->texture->llen);
		y++;
	}
}

static void	_render_3d_column(t_game *g, int x)
{
	t_raycast	rc;
	t_column	col;
	double		camera_x;

	rc = (t_raycast){0};
	camera_x = 2.0 * x / g->frame.width - 1.0;
	rc.ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	rc.ray_y = g->player.dir_y + g->player.plane_y * camera_x;
	_render_background(g, x);
	if (raycast_cast(g, &rc))
	{
		_render_column_setup(g, &rc, &col);
		_render_wall(g, x, &col);
	}
}

void	render_3d(t_game *g)
{
	int	x;

	x = 0;
	while (x < g->frame.width)
	{
		_render_3d_column(g, x);
		x++;
	}
}
