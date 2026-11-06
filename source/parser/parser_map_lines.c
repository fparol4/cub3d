/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map_lines.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

/* Replaces the pointer array; existing row strings keep their ownership. */
static char	**_parser_resize_grid(t_parser_data *data)
{
	char	**grid;
	size_t	count;

	count = (size_t)data->map.height + 2;
	grid = malloc(sizeof(*grid) * count);
	if (!grid)
		return (NULL);
	if (data->map.grid)
		ft_memcpy(grid, data->map.grid, ((size_t)data->map.height + 1)
			* sizeof(*grid));
	free(data->map.grid);
	return (grid);
}

/* Takes ownership of line only after the resized grid is available. */
int	parser_store_map_line(t_parser_data *data, char *line)
{
	char	**grid;
	size_t	length;

	length = ft_strlen(line);
	if (data->map.height < 0 || data->map.height == INT_MAX
		|| (size_t)data->map.height > SIZE_MAX / sizeof(char *) - 2)
		return (0);
	if (length > INT_MAX)
		return (0);
	grid = _parser_resize_grid(data);
	if (!grid)
		return (0);
	data->map.grid = grid;
	data->map.grid[data->map.height++] = line;
	data->map.grid[data->map.height] = NULL;
	if ((int)length > data->map.width)
		data->map.width = (int)length;
	return (1);
}
