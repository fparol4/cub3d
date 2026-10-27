/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

int	hook_press(int keycode, t_game *g)
{
	int	key_id;

	if (keycode == KEY_ESC)
		return (runtime_close(g));
	key_id = common_key_id(keycode);
	if (key_id == GK_VIEW)
	{
		if (g->state.keys[GK_VIEW])
			return (0);
		g->state.keys[GK_VIEW] = 1;
		screen_toggle_view(g);
		return (0);
	}
	if (key_id >= 0 && key_id < GK_COUNT)
	{
		if (!g->state.keys[key_id])
			g->state.needs_redraw = 1;
		g->state.keys[key_id] = 1;
	}
	return (0);
}

int	hook_release(int keycode, t_game *g)
{
	int	key_id;

	key_id = common_key_id(keycode);
	if (key_id >= 0 && key_id < GK_COUNT)
	{
		if (g->state.keys[key_id])
			g->state.needs_redraw = 1;
		g->state.keys[key_id] = 0;
	}
	return (0);
}

int	hook_out(t_game *g)
{
	int	i;

	i = 0;
	while (i < GK_COUNT)
	{
		if (g->state.keys[i])
			g->state.needs_redraw = 1;
		g->state.keys[i++] = 0;
	}
	return (0);
}

int	hook_expose(t_game *g)
{
	g->state.needs_redraw = 1;
	return (0);
}
