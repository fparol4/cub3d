/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_connectivity.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "source/map.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

static int	_map_flood_add(t_map_flood *flood, int x, int y)
{
	size_t	index;

	if (!map_is_walkable(map_cell_at(flood->data, x, y)))
		return (1);
	index = (size_t)y * flood->data->map.width + x;
	if (flood->visited[index])
		return (1);
	if (flood->tail >= flood->capacity)
		return (0);
	flood->visited[index] = 1;
	flood->queue[flood->tail++] = (t_point){x, y};
	return (1);
}

static int	_map_flood_neighbours(t_map_flood *flood, t_point point)
{
	if (!_map_flood_add(flood, point.x - 1, point.y))
		return (0);
	if (!_map_flood_add(flood, point.x + 1, point.y))
		return (0);
	if (!_map_flood_add(flood, point.x, point.y - 1))
		return (0);
	return (_map_flood_add(flood, point.x, point.y + 1));
}

static int	_map_flood_init(t_map_flood *flood, t_map_data *data)
{
	size_t	cells;

	if ((size_t)data->map.height > SIZE_MAX / (size_t)data->map.width)
		return (0);
	cells = (size_t)data->map.width * data->map.height;
	if (cells > SIZE_MAX / sizeof(*flood->queue))
		return (0);
	flood->data = data;
	flood->capacity = cells;
	flood->tail = 1;
	flood->visited = ft_calloc(cells, sizeof(*flood->visited));
	flood->queue = malloc(cells * sizeof(*flood->queue));
	if (!flood->visited || !flood->queue)
		return (free(flood->visited), free(flood->queue), 0);
	flood->visited[(size_t)data->player_y * data->map.width
		+ data->player_x] = 1;
	flood->queue[0] = (t_point){data->player_x, data->player_y};
	return (1);
}

static size_t	_map_flood_run(t_map_flood *flood)
{
	t_point	point;
	size_t	head;

	head = 0;
	while (head < flood->tail)
	{
		point = flood->queue[head++];
		if (!_map_flood_neighbours(flood, point))
			return (0);
	}
	return (flood->tail);
}

int	map_connectivity_validate(t_map_data *data)
{
	t_map_flood	flood;
	size_t		reachable;
	size_t		total;

	flood = (t_map_flood){0};
	if (!_map_flood_init(&flood, data))
		return (-1);
	reachable = _map_flood_run(&flood);
	total = map_count_walkable(data);
	free(flood.visited);
	free(flood.queue);
	return (reachable == total);
}
