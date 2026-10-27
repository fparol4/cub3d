/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_parser_is_closed_cell(t_parser_data *data, int x, int y)
{
	char	cell;

	cell = parser_map_at(data, x - 1, y);
	if (cell != T_WALL && !parser_is_walkable(cell))
		return (0);
	cell = parser_map_at(data, x + 1, y);
	if (cell != T_WALL && !parser_is_walkable(cell))
		return (0);
	cell = parser_map_at(data, x, y - 1);
	if (cell != T_WALL && !parser_is_walkable(cell))
		return (0);
	cell = parser_map_at(data, x, y + 1);
	return (cell == T_WALL || parser_is_walkable(cell));
}

static int	_parser_inspect_map_line(t_parser_data *data, int y, int *players)
{
	int	x;
	int	cell;

	x = 0;
	while (data->map.grid[y][x])
	{
		cell = data->map.grid[y][x];
		if (cell != T_FLOOR && cell != T_WALL && cell != ' '
			&& !parser_is_player(cell))
			return (0);
		if (parser_is_player(cell))
		{
			(*players)++;
			data->player_x = x;
			data->player_y = y;
			data->player_dir = cell;
		}
		x++;
	}
	return (1);
}

static int	_parser_validate_closed_cells(t_parser_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (parser_is_walkable(parser_map_at(data, x, y))
				&& !_parser_is_closed_cell(data, x, y))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

static int	_parser_count_players(t_parser_data *data)
{
	int	y;
	int	players;

	y = 0;
	players = 0;
	while (y < data->map.height)
	{
		if (!_parser_inspect_map_line(data, y, &players))
			return (-1);
		y++;
	}
	return (players);
}

int	parser_valid_map(t_parser_data *data)
{
	if (!data || (data->fields & CUB_REQ_FIELDS) != CUB_REQ_FIELDS)
		return (0);
	if (!data->map.grid || data->map.height < 3 || data->map.width < 3)
		return (0);
	if (_parser_count_players(data) != 1 || !_parser_validate_closed_cells(data))
		return (0);
	return (parser_validate_map_connectivity(data) == 1);
}
