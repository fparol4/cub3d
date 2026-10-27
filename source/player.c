/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
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
	double	rd;

	rd = PLAYER_RADIUS;
	return (!_player_cell_blocked(g, x + rd, y + rd)
		&& !_player_cell_blocked(g, x + rd, y - rd)
		&& !_player_cell_blocked(g, x - rd, y + rd)
		&& !_player_cell_blocked(g, x - rd, y - rd));
}

static void	_player_move(t_game *g, double elapsed)
{
	double	fw;
	double	st;
	double	ln;
	double	mx;
	double	my;

	fw = g->state.keys[GK_FORWARD] - g->state.keys[GK_BACKWARD];
	st = g->state.keys[GK_LEFT] - g->state.keys[GK_RIGHT];
	ln = sqrt(fw * fw + st * st);
	if (ln == 0.0)
		return ;
	fw /= ln;
	st /= ln;
	mx = (g->player.dir_x * fw + g->player.dir_y * st) * PLAYER_SPEED * elapsed;
	my = (g->player.dir_y * fw - g->player.dir_x * st) * PLAYER_SPEED * elapsed;
	if (_player_position_valid(g, g->player.pos_x + mx, g->player.pos_y))
		g->player.pos_x += mx;
	if (_player_position_valid(g, g->player.pos_x, g->player.pos_y + my))
		g->player.pos_y += my;
}

static void	_player_rotate(t_player *p, double angle)
{
	double	ox;
	double	cs;
	double	sn;

	ox = p->dir_x;
	cs = cos(angle);
	sn = sin(angle);
	p->dir_x = p->dir_x * cs - p->dir_y * sn;
	p->dir_y = ox * sn + p->dir_y * cs;
	ox = p->plane_x;
	p->plane_x = p->plane_x * cs - p->plane_y * sn;
	p->plane_y = ox * sn + p->plane_y * cs;
}

void	player_update(t_game *g, double elapsed)
{
	_player_move(g, elapsed);
	if (g->state.keys[GK_ACLOCKWISE])
		_player_rotate(&g->player, -PLAYER_ROTATION_SPEED * elapsed);
	if (g->state.keys[GK_CLOCKWISE])
		_player_rotate(&g->player, PLAYER_ROTATION_SPEED * elapsed);
}
