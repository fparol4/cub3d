/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "source/common.h"
#include "source/map.h"
#include <stdlib.h>

static void	_map_apply_player(t_game *g, t_map_data *data)
{
	double	dir_x;
	double	dir_y;

	dir_x = 0.0;
	dir_y = 0.0;
	if (data->player_dir == 'N')
		dir_y = -1.0;
	else if (data->player_dir == 'S')
		dir_y = 1.0;
	else if (data->player_dir == 'E')
		dir_x = 1.0;
	else if (data->player_dir == 'W')
		dir_x = -1.0;
	g->player.pos_x = data->player_x + 0.5;
	g->player.pos_y = data->player_y + 0.5;
	g->player.dir_x = dir_x;
	g->player.dir_y = dir_y;
	g->player.plane_x = -dir_y * PLAYER_PLANE_SCALE;
	g->player.plane_y = dir_x * PLAYER_PLANE_SCALE;
}

static void	_map_transfer(t_game *g, t_map_data *data)
{
	g->map = data->map;
	data->map = (t_map){0};
	g->map.grid[data->player_y][data->player_x] = T_FLOOR;
	_map_apply_player(g, data);
}

static int	_map_parse_data(t_map_data *data, const char *path)
{
	if (!map_read_archive(data, path))
		return (0);
	if (!map_validate(data))
		return (0);
	return (1);
}

void	map_cleanup(t_map *map)
{
	int	y;

	if (!map)
		return ;
	y = 0;
	while (map->grid && y < map->height)
		free(map->grid[y++]);
	free(map->grid);
	free(map->no_texture);
	free(map->so_texture);
	free(map->we_texture);
	free(map->ea_texture);
	*map = (t_map){0};
}

int	map_parse(t_game *g, char *map_path)
{
	t_map_data	data;

	if (!g || !map_path)
		return (common_error("Invalid map file"));
	data = (t_map_data){0};
	if (!_map_parse_data(&data, map_path))
		return (map_cleanup(&data.map), common_error("Invalid map file"));
	_map_transfer(g, &data);
	map_cleanup(&data.map);
	return (0);
}
