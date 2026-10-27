/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_raycast_setup(t_game *g, t_raycast *rc)
{
	rc->state.map_x = (int)floor(g->player.pos_x);
	rc->state.map_y = (int)floor(g->player.pos_y);
	rc->state.delta_x = 1e30;
	rc->state.delta_y = 1e30;
	if (rc->ray_x != 0.0)
		rc->state.delta_x = fabs(1.0 / rc->ray_x);
	if (rc->ray_y != 0.0)
		rc->state.delta_y = fabs(1.0 / rc->ray_y);
	rc->state.step_x = 1;
	rc->state.step_y = 1;
	if (rc->ray_x < 0.0)
		rc->state.step_x = -1;
	if (rc->ray_y < 0.0)
		rc->state.step_y = -1;
	rc->state.side_x = fabs(rc->state.map_x + (rc->state.step_x > 0)
			- g->player.pos_x) * rc->state.delta_x;
	rc->state.side_y = fabs(rc->state.map_y + (rc->state.step_y > 0)
			- g->player.pos_y) * rc->state.delta_y;
}

static void	_raycast_advance(t_raycast *rc)
{
	if (rc->state.side_x < rc->state.side_y)
	{
		rc->hit.distance = rc->state.side_x;
		rc->state.side_x += rc->state.delta_x;
		rc->state.map_x += rc->state.step_x;
		rc->state.side = RAYSIDE_X;
	}
	else
	{
		rc->hit.distance = rc->state.side_y;
		rc->state.side_y += rc->state.delta_y;
		rc->state.map_y += rc->state.step_y;
		rc->state.side = RAYSIDE_Y;
	}
}

static int	_raycast_in_boundaries(t_game *g, t_raycast *rc)
{
	return (rc->state.map_x >= 0 && rc->state.map_y >= 0
		&& rc->state.map_x < g->map.width && rc->state.map_y < g->map.height);
}

static int	_raycast_is_hit(t_game *g, t_raycast *rc)
{
	if (g->map.grid[rc->state.map_y][rc->state.map_x] == T_FLOOR)
		return (0);
	rc->hit.side = rc->state.side;
	rc->hit.x = g->player.pos_x + rc->ray_x * rc->hit.distance;
	rc->hit.y = g->player.pos_y + rc->ray_y * rc->hit.distance;
	return (1);
}

int	raycast_cast(t_game *g, t_raycast *rc)
{
	int	steps;

	steps = 0;
	rc->hit = (t_ray_hit){0};
	_raycast_setup(g, rc);
	while (steps++ < g->map.width + g->map.height)
	{
		_raycast_advance(rc);
		if (!_raycast_in_boundaries(g, rc))
			return (0);
		if (_raycast_is_hit(g, rc))
			return (1);
	}
	return (0);
}
