/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "libft.h"
#include "mlx.h"
#include "source/common.h"
#include "source/game.h"
#include "source/hook.h"
#include "source/runtime.h"
#include "source/setup.h"
#include "source/texture.h"

static int	_setup_mlx(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (common_error("Cannot initialize graphics"));
	g->win = mlx_new_window(g->mlx, S_WIDTH, S_HEIGHT, G_NAME);
	if (!g->win)
		return (common_error("Cannot initialize graphics"));
	return (0);
}

static void	_setup_hook(t_game *g)
{
	mlx_hook(g->win, EVT_KEY_PRESS, MASK_KEY_PRESS, hook_press, g);
	mlx_hook(g->win, EVT_KEY_RELEASE, MASK_KEY_RELEASE, hook_release, g);
	mlx_hook(g->win, EVT_FOCUS_OUT, MASK_FOCUS_CHANGE, hook_out, g);
	mlx_hook(g->win, EVT_EXPOSE, MASK_EXPOSURE, hook_expose, g);
	mlx_hook(g->win, EVT_EXIT, MASK_STRUCTURE_NOTIFY, runtime_close, g);
	mlx_loop_hook(g->mlx, game_frame, g);
}

static int	_setup_frame(t_game *g)
{
	g->frame.width = S_WIDTH;
	g->frame.height = S_HEIGHT;
	g->frame.ptr = mlx_new_image(g->mlx, g->frame.width, g->frame.height);
	if (!g->frame.ptr)
		return (common_error("Cannot initialize graphics"));
	g->frame.addr = mlx_get_data_addr(g->frame.ptr, &g->frame.bpp,
			&g->frame.llen, &g->frame.endian);
	if (!g->frame.addr)
		return (common_error("Cannot initialize graphics"));
	ft_memset(g->frame.addr, 0, g->frame.llen * g->frame.height);
	return (0);
}

int	setup_game(t_game *g)
{
	if (_setup_mlx(g) || texture_load(g) || _setup_frame(g))
		return (1);
	_setup_hook(g);
	return (0);
}
