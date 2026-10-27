/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fov.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:19 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_render_fov_ray(t_game *g, t_viewport *v, int i, t_point from)
{
	t_raycast	ray;
	t_point		hit;
	double		camera_x;

	ray = (t_raycast){0};
	camera_x = 0.0;
	if (RENDER_FOV_RAYS > 1)
		camera_x = (2.0 * i / (RENDER_FOV_RAYS - 1.0)) - 1.0;
	ray.ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray.ray_y = g->player.dir_y + g->player.plane_y * camera_x;
	if (raycast_cast(g, &ray))
	{
		hit = render_map_point(g, v, ray.hit.x, ray.hit.y);
		render_line(&g->graphics.frame, from, hit, COLOR_FOV);
	}
}

void	render_fov(t_game *g, t_viewport *v)
{
	int		i;
	t_point	from;

	from = render_map_point(g, v, g->player.pos_x, g->player.pos_y);
	i = 0;
	while (i < RENDER_FOV_RAYS)
	{
		_render_fov_ray(g, v, i, from);
		i++;
	}
}
