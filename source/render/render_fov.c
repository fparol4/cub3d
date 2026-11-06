/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fov.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_render_fov_ray(t_game *g, t_viewport *v, int i, t_point from)
{
	t_raycast	ray;
	t_point		hit;

	raycast_camera_ray(g, &ray, i);
	if (raycast_cast(g, &ray))
	{
		hit = render_world_to_map(g, v, ray.hit.x, ray.hit.y);
		render_line(&g->graphics.frame, from, hit, COLOR_FOV);
	}
}

void	render_fov(t_game *g, t_viewport *v)
{
	int		i;
	t_point	from;

	from = render_world_to_map(g, v, g->player.pos_x, g->player.pos_y);
	i = 0;
	while (i < RENDER_FOV_RAYS)
	{
		_render_fov_ray(g, v, i, from);
		i++;
	}
}
