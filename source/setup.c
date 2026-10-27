/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_setup_mlx(t_game *g)
{
	g->graphics.mlx = mlx_init();
	if (!g->graphics.mlx)
		return (common_error("Cannot initialize graphics"));
	g->graphics.window = mlx_new_window(g->graphics.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, G_NAME);
	if (!g->graphics.window)
		return (common_error("Cannot initialize graphics"));
	return (0);
}

static void	_setup_hook(t_game *g)
{
	mlx_hook(g->graphics.window, EVT_KEY_PRESS, MASK_KEY_PRESS, hook_press, g);
	mlx_hook(g->graphics.window, EVT_KEY_RELEASE, MASK_KEY_RELEASE, hook_release, g);
	mlx_hook(g->graphics.window, EVT_FOCUS_OUT, MASK_FOCUS_CHANGE, hook_out, g);
	mlx_hook(g->graphics.window, EVT_EXPOSE, MASK_EXPOSURE, hook_expose, g);
	mlx_hook(g->graphics.window, EVT_EXIT, MASK_STRUCTURE_NOTIFY, runtime_close, g);
	mlx_loop_hook(g->graphics.mlx, game_frame, g);
}

static int	_setup_frame(t_game *g)
{
	g->graphics.frame.width = SCREEN_WIDTH;
	g->graphics.frame.height = SCREEN_HEIGHT;
	g->graphics.frame.ptr = mlx_new_image(g->graphics.mlx, g->graphics.frame.width, g->graphics.frame.height);
	if (!g->graphics.frame.ptr)
		return (common_error("Cannot initialize graphics"));
	g->graphics.frame.addr = mlx_get_data_addr(g->graphics.frame.ptr, &g->graphics.frame.bpp,
			&g->graphics.frame.stride, &g->graphics.frame.endian);
	if (!g->graphics.frame.addr)
		return (common_error("Cannot initialize graphics"));
	ft_memset(g->graphics.frame.addr, 0, g->graphics.frame.stride * g->graphics.frame.height);
	return (0);
}

int	setup_game(t_game *g)
{
	if (_setup_mlx(g) || texture_load(g) || _setup_frame(g))
		return (1);
	_setup_hook(g);
	return (0);
}
