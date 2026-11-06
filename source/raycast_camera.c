/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_camera.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

void	raycast_camera_ray(t_game *g, t_raycast *ray, int index)
{
	double	camera_x;

	*ray = (t_raycast){0};
	camera_x = 0.0;
	if (RENDER_FOV_RAYS > 1)
		camera_x = 2.0 * index / (RENDER_FOV_RAYS - 1.0) - 1.0;
	ray->ray_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray->ray_y = g->player.dir_y + g->player.plane_y * camera_x;
}
