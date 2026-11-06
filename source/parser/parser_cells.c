/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cells.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:39 by fcardozo         ###   ########.fr       */
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
