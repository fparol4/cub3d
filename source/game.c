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

static void	_game_capframe(double *elapsed)
{
	if (*elapsed < 0.0)
		*elapsed = 0;
	if (*elapsed > PLAYER_MAX_ELAPSED)
		*elapsed = PLAYER_MAX_ELAPSED;
}

int	game_frame(t_game *g)
{
	double	now;
	double	elapsed;

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
	_game_capframe(&elapsed);
	player_update(g, elapsed);
	if (screen_clear(g) || screen_render(g))
	{
		g->status = common_error("Cannot render frame");
		return (runtime_close(g));
	}
	return (0);
}
