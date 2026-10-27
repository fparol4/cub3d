/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_wall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_render_background(t_game *g, int x)
{
	char	*dst;
	int		y;
	int		color;

	dst = g->graphics.frame.addr + x * (g->graphics.frame.bpp / 8);
	y = 0;
	while (y < g->graphics.frame.height)
	{
		color = g->map.color_floor;
		if (y < g->graphics.frame.height / 2)
			color = g->map.color_ceiling;
		*(unsigned int *)(dst + y * g->graphics.frame.stride) = (unsigned int)color;
		y++;
	}
}

static void	_render_column_setup(t_game *g, t_raycast *rc, t_column *col)
{
	double	distance;
	double	offset;

	col->texture = texture_face(g, rc);
	distance = fmax(rc->hit.distance, RENDER_MIN_DISTANCE);
	col->height = (int)(g->graphics.frame.height / distance);
	col->top = g->graphics.frame.height / 2 - col->height / 2;
	col->start = (int)fmax(col->top, 0);
	col->end = (int)fmin(col->top + col->height, g->graphics.frame.height);
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

	dst = g->graphics.frame.addr + x * (g->graphics.frame.bpp / 8);
	src = col->texture->addr + col->tex_x * (col->texture->bpp / 8);
	y = col->start;
	while (y < col->end)
	{
		tex_y = (int)((double)(y - col->top) * col->texture->height
				/ col->height);
		*(unsigned int *)(dst + y * g->graphics.frame.stride) = *(unsigned int *)
			(src + tex_y * col->texture->stride);
		y++;
	}
}

static void	_render_3d_ray(t_game *g, int i)
{
	t_raycast	rc;
	t_column	col;
	double		camera_x;
	int			x;
	int			end;
	int			hit;

	rc = (t_raycast){0};
	camera_x = 0.0;
	if (RENDER_FOV_RAYS > 1)
		camera_x = 2.0 * i / (RENDER_FOV_RAYS - 1.0) - 1.0;
	rc.ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	rc.ray_y = g->player.dir_y + g->player.plane_y * camera_x;
	hit = raycast_cast(g, &rc);
	if (hit)
		_render_column_setup(g, &rc, &col);
	x = (int)((long long)i * g->graphics.frame.width / RENDER_FOV_RAYS);
	end = (int)(((long long)i + 1) * g->graphics.frame.width / RENDER_FOV_RAYS);
	while (x < end)
	{
		_render_background(g, x);
		if (hit)
			_render_wall(g, x, &col);
		x++;
	}
}

void	render_3d(t_game *g)
{
	int	i;

	i = 0;
	while (i < RENDER_FOV_RAYS)
	{
		_render_3d_ray(g, i);
		i++;
	}
}
