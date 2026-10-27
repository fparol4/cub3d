/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_parser_apply_player(t_game *g, t_parser_data *data)
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

static void	_parser_transfer(t_game *g, t_parser_data *data)
{
	g->map = data->map;
	data->map = (t_map){0};
	g->map.grid[data->player_y][data->player_x] = T_FLOOR;
	_parser_apply_player(g, data);
}

void	parser_cleanup(t_map *map)
{
	int	y;

	if (!map)
		return ;
	y = 0;
	while (map->grid && y < map->height)
		free(map->grid[y++]);
	free(map->grid);
	y = 0;
	while (y < TEX_COUNT)
		free(map->texture_path[y++]);
	*map = (t_map){0};
}

int	parser_controller(t_game *g, char *map_path)
{
	t_parser_data	data;

	if (!g || !map_path)
		return (common_error("Invalid map file"));
	data = (t_parser_data){0};
	if (!parser_split_archive(&data, map_path) || !parser_valid_map(&data))
		return (parser_cleanup(&data.map), common_error("Invalid map file"));
	_parser_transfer(g, &data);
	return (0);
}
