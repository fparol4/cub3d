/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "libft.h"
#include "source/map.h"
#include <stdlib.h>

static void	_map_player(t_game *g)
{
	g->player.pos_x = 3.5;
	g->player.pos_y = 2.5;
	g->player.dir_x = 1.0;
	g->player.dir_y = 0.0;
	g->player.plane_x = 0.0;
	g->player.plane_y = PLAYER_PLANE_SCALE;
}

static int	_map_rows(t_game *g)
{
	int					i;
	static const char	*rows[] = {"11111111", "10000001", "10000001",
		"10000001", "10000001", "10000001", "11111111"};

	g->map.width = 8;
	g->map.height = 7;
	g->map.grid = ft_calloc(g->map.height, sizeof(*g->map.grid));
	if (!g->map.grid)
		return (1);
	i = 0;
	while (i < g->map.height)
	{
		g->map.grid[i] = ft_strdup(rows[i]);
		if (!g->map.grid[i])
			return (1);
		i++;
	}
	return (0);
}

void	map_cleanup(t_map *m)
{
	int	i;

	i = 0;
	while (m->grid && i < m->height)
		free(m->grid[i++]);
	free(m->grid);
	free(m->no_texture);
	free(m->so_texture);
	free(m->we_texture);
	free(m->ea_texture);
	*m = (t_map){0};
}

int	map_parse(t_game *g, char *map_path)
{
	(void)map_path;
	g->map.color_floor = 0x00323232;
	g->map.color_ceiling = 0x006EA0D2;
	g->map.no_texture = ft_strdup("textures/north.xpm");
	g->map.so_texture = ft_strdup("textures/south.xpm");
	g->map.we_texture = ft_strdup("textures/west.xpm");
	g->map.ea_texture = ft_strdup("textures/east.xpm");
	if (!g->map.no_texture || !g->map.so_texture || !g->map.we_texture
		|| !g->map.ea_texture)
		return (map_cleanup(&g->map), 1);
	if (_map_rows(g))
		return (map_cleanup(&g->map), 1);
	_map_player(g);
	return (0);
}
