/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_cells.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/enum.h"
#include "source/map.h"

int	map_is_player(char cell)
{
	return (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W');
}

int	map_is_walkable(char cell)
{
	return (cell == T_FLOOR || map_is_player(cell));
}

/* The reader pads every row to map.width before validation uses this helper. */
char	map_cell_at(t_map_data *data, int x, int y)
{
	if (!data || !data->map.grid || x < 0 || y < 0 || x >= data->map.width
		|| y >= data->map.height)
		return (' ');
	return (data->map.grid[y][x]);
}

size_t	map_count_walkable(t_map_data *data)
{
	int		x;
	int		y;
	size_t	count;

	y = 0;
	count = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (map_is_walkable(map_cell_at(data, x, y)))
				count++;
			x++;
		}
		y++;
	}
	return (count);
}
