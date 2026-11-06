/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_player_cell_blocked(t_game *g, double x, double y)
{
	int	cell_x;
	int	cell_y;

	cell_x = (int)floor(x);
	cell_y = (int)floor(y);
	if (cell_x < 0 || cell_x >= g->map.width)
		return (1);
	if (cell_y < 0 || cell_y >= g->map.height)
		return (1);
	return (g->map.grid[cell_y][cell_x] != T_FLOOR);
}

static int	_player_position_valid(t_game *g, double x, double y)
{
	double	radius;

	radius = PLAYER_RADIUS;
	return (!_player_cell_blocked(g, x + radius, y + radius)
		&& !_player_cell_blocked(g, x + radius, y - radius)
		&& !_player_cell_blocked(g, x - radius, y + radius)
		&& !_player_cell_blocked(g, x - radius, y - radius));
}

static void	_player_move(t_game *g, double elapsed)
{
	double	forward;
	double	strafe;
	double	length;
	double	move_x;
	double	move_y;

	forward = g->state.keys[GK_FORWARD] - g->state.keys[GK_BACKWARD];
	strafe = g->state.keys[GK_STRAFE_LEFT] - g->state.keys[GK_STRAFE_RIGHT];
	length = sqrt(forward * forward + strafe * strafe);
	if (length == 0.0)
		return ;
	forward /= length;
	strafe /= length;
	move_x = (g->player.dir_x * forward + g->player.dir_y * strafe)
		* PLAYER_SPEED * elapsed;
	move_y = (g->player.dir_y * forward - g->player.dir_x * strafe)
		* PLAYER_SPEED * elapsed;
	if (_player_position_valid(g, g->player.pos_x + move_x, g->player.pos_y))
		g->player.pos_x += move_x;
	if (_player_position_valid(g, g->player.pos_x, g->player.pos_y + move_y))
		g->player.pos_y += move_y;
}

static void	_player_rotate(t_player *p, double angle)
{
	double	old_x;
	double	cos_angle;
	double	sin_angle;

	old_x = p->dir_x;
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	p->dir_x = p->dir_x * cos_angle - p->dir_y * sin_angle;
	p->dir_y = old_x * sin_angle + p->dir_y * cos_angle;
	old_x = p->plane_x;
	p->plane_x = p->plane_x * cos_angle - p->plane_y * sin_angle;
	p->plane_y = old_x * sin_angle + p->plane_y * cos_angle;
}

void	player_update(t_game *g, double elapsed)
{
	_player_move(g, elapsed);
	if (g->state.keys[GK_TURN_LEFT])
		_player_rotate(&g->player, -PLAYER_ROTATION_SPEED * elapsed);
	if (g->state.keys[GK_TURN_RIGHT])
		_player_rotate(&g->player, PLAYER_ROTATION_SPEED * elapsed);
}
