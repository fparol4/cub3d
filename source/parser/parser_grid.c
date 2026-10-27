/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_grid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static void	_parser_free_grid(char **grid, int height)
{
	int	y;

	y = 0;
	while (grid && y < height)
		free(grid[y++]);
	free(grid);
}

static char	*_parser_pad_row(const char *row, int width)
{
	char	*padded;
	int		x;

	padded = malloc((size_t)width + 1);
	if (!padded)
		return (NULL);
	x = 0;
	while (x < width && row[x])
	{
		padded[x] = row[x];
		x++;
	}
	while (x < width)
		padded[x++] = ' ';
	padded[x] = '\0';
	return (padded);
}

static char	**_parser_copy_grid(t_map *map)
{
	char	**grid;
	int		y;

	grid = ft_calloc((size_t)map->height + 1, sizeof(*grid));
	if (!grid)
		return (NULL);
	y = 0;
	while (y < map->height)
	{
		grid[y] = _parser_pad_row(map->grid[y], map->width);
		if (!grid[y])
			return (_parser_free_grid(grid, map->height), NULL);
		y++;
	}
	return (grid);
}

int	parser_rectangularize(t_map *map)
{
	char	**grid;

	grid = _parser_copy_grid(map);
	if (!grid)
		return (0);
	_parser_free_grid(map->grid, map->height);
	map->grid = grid;
	return (1);
}
