/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/enum.h"
#include "source/map.h"

static int	_map_cell_closed(t_map_data *data, int x, int y)
{
	char	cell;

	cell = map_cell_at(data, x - 1, y);
	if (cell != T_WALL && !map_is_walkable(cell))
		return (0);
	cell = map_cell_at(data, x + 1, y);
	if (cell != T_WALL && !map_is_walkable(cell))
		return (0);
	cell = map_cell_at(data, x, y - 1);
	if (cell != T_WALL && !map_is_walkable(cell))
		return (0);
	cell = map_cell_at(data, x, y + 1);
	return (cell == T_WALL || map_is_walkable(cell));
}

static int	_map_inspect_line(t_map_data *data, int y, int *players)
{
	int	x;
	int	cell;

	x = 0;
	while (data->map.grid[y][x])
	{
		cell = data->map.grid[y][x];
		if (cell != T_FLOOR && cell != T_WALL && cell != ' '
			&& !map_is_player(cell))
			return (0);
		if (map_is_player(cell))
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

static int	_map_cells_closed(t_map_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (map_is_walkable(map_cell_at(data, x, y))
				&& !_map_cell_closed(data, x, y))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

static int	_map_player_count(t_map_data *data)
{
	int	y;
	int	players;

	y = 0;
	players = 0;
	while (y < data->map.height)
	{
		if (!_map_inspect_line(data, y, &players))
			return (-1);
		y++;
	}
	return (players);
}

int	map_validate(t_map_data *data)
{
	if (!data || (data->fields & CUB_REQ_FIELDS) != CUB_REQ_FIELDS)
		return (0);
	if (!data->map.grid || data->map.height < 3 || data->map.width < 3)
		return (0);
	if (_map_player_count(data) != 1 || !_map_cells_closed(data))
		return (0);
	return (map_connectivity_validate(data) == 1);
}
