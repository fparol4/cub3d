/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mlx.h"
#include "source/common.h"
#include "source/map.h"
#include "source/runtime.h"
#include "source/setup.h"
#include "source/texture.h"
#include <stdlib.h>

int	runtime_args(int argc, char **argv)
{
	int		l;
	char	*m;

	m = "Usage: ./cub3D <map.cub>";
	if (argc != 2)
		return (common_error(m));
	l = ft_strlen(argv[1]);
	if (l < 4 || ft_strcmp((argv[1] + l - 4), ".cub"))
		return (common_error(m));
	return (0);
}

void	runtime_cleanup(t_game *g)
{
	texture_cleanup(g);
	if (g->frame.ptr)
		mlx_destroy_image(g->mlx, g->frame.ptr);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	map_cleanup(&g->map);
	g->frame = (t_img){0};
	g->win = NULL;
	g->mlx = NULL;
}

int	runtime_setup(t_game *g, char *map_path)
{
	*g = (t_game){0};
	if (map_parse(g, map_path))
		return (runtime_cleanup(g), 1);
	if (setup_game(g))
		return (runtime_cleanup(g), 1);
	return (0);
}

int	runtime_run(t_game *g)
{
	mlx_loop(g->mlx);
	runtime_cleanup(g);
	return (g->status);
}

int	runtime_close(t_game *g)
{
	mlx_loop_end(g->mlx);
	return (0);
}
