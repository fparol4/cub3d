/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "source/common.h"
#include "source/game.h"
#include "source/player.h"
#include "source/runtime.h"
#include "source/screen.h"

static int	_game_has_motion(t_game *g)
{
	return (g->keys[GK_FORWARD] || g->keys[GK_BACKWARD]
		|| g->keys[GK_LEFT] || g->keys[GK_RIGHT]
		|| g->keys[GK_CLOCKWISE] || g->keys[GK_ACLOCKWISE]);
}

static int	_game_player_changed(t_player *old, t_player *current)
{
	return (old->pos_x != current->pos_x || old->pos_y != current->pos_y
		|| old->dir_x != current->dir_x || old->dir_y != current->dir_y
		|| old->plane_x != current->plane_x
		|| old->plane_y != current->plane_y);
}

static void	_game_update_player(t_game *g, double elapsed)
{
	t_player	previous;

	previous = g->player;
	player_update(g, elapsed);
	if (_game_player_changed(&previous, &g->player))
		g->dirty = 1;
}

static int	_game_render(t_game *g)
{
	if (!g->dirty)
		return (0);
	if (screen_clear(g) || screen_render(g))
	{
		g->status = common_error("Cannot render frame");
		return (runtime_close(g));
	}
	g->dirty = 0;
	return (0);
}

int	game_frame(t_game *g)
{
	double	now;
	double	elapsed;

	if (!g->dirty && !_game_has_motion(g))
		return (0);
	now = common_now();
	if (now < 0.0)
	{
		g->status = common_error("Cannot read frame time");
		return (runtime_close(g));
	}
	if (g->last_frame == 0.0)
		elapsed = 0.0;
	else
		elapsed = now - g->last_frame;
	g->last_frame = now;
	if (elapsed < 0.0)
		elapsed = 0.0;
	if (elapsed > PLAYER_MAX_ELAPSED)
		elapsed = PLAYER_MAX_ELAPSED;
	if (_game_has_motion(g))
		_game_update_player(g, elapsed);
	return (_game_render(g));
}
