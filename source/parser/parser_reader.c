/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_parser_is_start(const char *line)
{
	if ((line[0] == 'N' && line[1] == 'O')
		|| (line[0] == 'S' && line[1] == 'O')
		|| (line[0] == 'W' && line[1] == 'E')
		|| (line[0] == 'E' && line[1] == 'A'))
		return (0);
	return (*line == T_WALL || parser_is_walkable(*line));
}

static char	**_parser_resize_grid(t_parser_data *data)
{
	char	**grid;
	size_t	count;

	count = (size_t)data->map.height + 2;
	grid = malloc(sizeof(*grid) * count);
	if (!grid)
		return (NULL);
	if (data->map.grid)
		ft_memcpy(grid, data->map.grid,
			((size_t)data->map.height + 1) * sizeof(*grid));
	free(data->map.grid);
	return (grid);
}

static int	_parser_append_map_line(t_parser_data *data, char *line)
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

static int	_parser_process_line(t_parser_data *data, char *line)
{
	char	*trimmed;
	size_t	length;

	trimmed = ft_strtrim(line, " \t\r\n");
	if (!trimmed)
		return (free(line), 0);
	if (!data->map_started && !*trimmed)
		return (free(trimmed), free(line), 1);
	if (data->map_started || _parser_is_start(trimmed))
	{
		data->map_started = 1;
		free(trimmed);
		length = ft_strlen(line);
		if (length && line[length - 1] == '\n')
			line[length - 1] = '\0';
		if (!_parser_append_map_line(data, line))
			return (free(line), 0);
		return (1);
	}
	if (!parser_parse_config(data, trimmed))
		return (free(trimmed), free(line), 0);
	free(trimmed);
	free(line);
	return (1);
}

int	parser_split_archive(t_parser_data *data, const char *path)
{
	int		fd;
	char	*line;
	int		ok;

	if (!data || !path || ft_strlen(path) < 4
		|| ft_strncmp(path + ft_strlen(path) - 4, ".cub", 4))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	ok = 1;
	line = ft_get_next_line(fd);
	while (line && ok)
	{
		ok = _parser_process_line(data, line);
		if (ok)
			line = ft_get_next_line(fd);
	}
	close(fd);
	if (!ok || !data->map.height || !data->map.width)
		return (0);
	return (parser_rectangularize(&data->map));
}
