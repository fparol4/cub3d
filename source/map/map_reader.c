/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "source/map.h"
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

static char	*_map_trim_copy(const char *line)
{
	return (ft_strtrim(line, " \t\r\n"));
}

static int	_map_is_start(const char *line)
{
	if ((line[0] == 'N' && line[1] == 'O')
		|| (line[0] == 'S' && line[1] == 'O')
		|| (line[0] == 'W' && line[1] == 'E')
		|| (line[0] == 'E' && line[1] == 'A'))
		return (0);
	return (line && (*line == '0' || *line == '1' || *line == 'N'
			|| *line == 'S' || *line == 'E' || *line == 'W'));
}

static int	_map_append_line(t_map_data *data, char *line)
{
	char	**grid;

	if (data->map.height < 0 || data->map.height == INT_MAX
		|| (size_t)data->map.height > SIZE_MAX / sizeof(char *) - 2)
		return (0);
	if (ft_strlen(line) > INT_MAX)
		return (0);
	grid = realloc(data->map.grid,
			sizeof(char *) * ((size_t)data->map.height + 2));
	if (!grid)
		return (0);
	data->map.grid = grid;
	data->map.grid[data->map.height++] = line;
	data->map.grid[data->map.height] = NULL;
	if ((int)ft_strlen(line) > data->map.width)
		data->map.width = (int)ft_strlen(line);
	return (1);
}

static int	_map_process_line(t_map_data *data, char *line)
{
	char	*trimmed;
	size_t	length;

	trimmed = _map_trim_copy(line);
	if (!trimmed)
		return (free(line), 0);
	if (!data->map_started && !*trimmed)
		return (free(trimmed), free(line), 1);
	if (data->map_started || _map_is_start(trimmed))
	{
		data->map_started = 1;
		free(trimmed);
		length = ft_strlen(line);
		if (length && line[length - 1] == '\n')
			line[length - 1] = '\0';
		if (!_map_append_line(data, line))
			return (free(line), 0);
		return (1);
	}
	if (!map_parse_config(data, trimmed))
		return (free(trimmed), free(line), 0);
	free(trimmed);
	free(line);
	return (1);
}

int	map_read_archive(t_map_data *data, const char *path)
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
		ok = _map_process_line(data, line);
		if (ok)
			line = ft_get_next_line(fd);
	}
	close(fd);
	if (!ok || !data->map.height || !data->map.width)
		return (0);
	return (map_rectangularize(&data->map));
}
