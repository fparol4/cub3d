/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cells.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

int	parser_is_player(char cell)
{
	return (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W');
}

int	parser_is_walkable(char cell)
{
	return (cell == T_FLOOR || parser_is_player(cell));
}

char	parser_map_at(t_parser_data *data, int x, int y)
{
	if (!data || !data->map.grid || x < 0 || y < 0 || x >= data->map.width
		|| y >= data->map.height)
		return (' ');
	return (data->map.grid[y][x]);
}

size_t	parser_count_walkable_cells(t_parser_data *data)
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
			if (parser_is_walkable(parser_map_at(data, x, y)))
				count++;
			x++;
		}
		y++;
	}
	return (count);
}
