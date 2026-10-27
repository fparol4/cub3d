/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_game_has_motion(t_game *g)
{
	return (g->state.keys[GK_FORWARD] || g->state.keys[GK_BACKWARD]
		|| g->state.keys[GK_LEFT] || g->state.keys[GK_RIGHT]
		|| g->state.keys[GK_CLOCKWISE] || g->state.keys[GK_ACLOCKWISE]);
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
		g->state.needs_redraw = 1;
}

static int	_game_render(t_game *g)
{
	if (!g->state.needs_redraw)
		return (0);
	if (screen_clear(g) || screen_render(g))
	{
		g->state.exit_status = common_error("Cannot render frame");
		return (runtime_close(g));
	}
	g->state.needs_redraw = 0;
	return (0);
}

int	game_frame(t_game *g)
{
	double	now;
	double	elapsed;

	if (!g->state.needs_redraw && !_game_has_motion(g))
		return (0);
	now = common_now();
	if (now < 0.0)
	{
		g->state.exit_status = common_error("Cannot read frame time");
		return (runtime_close(g));
	}
	if (g->state.last_frame_time == 0.0)
		elapsed = 0.0;
	else
		elapsed = now - g->state.last_frame_time;
	g->state.last_frame_time = now;
	if (elapsed < 0.0)
		elapsed = 0.0;
	if (elapsed > PLAYER_MAX_ELAPSED)
		elapsed = PLAYER_MAX_ELAPSED;
	if (_game_has_motion(g))
		_game_update_player(g, elapsed);
	return (_game_render(g));
}
