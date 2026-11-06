/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

static int	_parser_is_map_start(const char *line)
{
	if ((line[0] == 'N' && line[1] == 'O') || (line[0] == 'S' && line[1] == 'O')
		|| (line[0] == 'W' && line[1] == 'E') || (line[0] == 'E'
			&& line[1] == 'A'))
		return (0);
	return (*line == T_WALL || parser_is_walkable(*line));
}

static int	_parser_process_map_line(t_parser_data *data, char *line)
{
	size_t	length;

	data->map_started = 1;
	length = ft_strlen(line);
	if (length && line[length - 1] == '\n')
		line[length - 1] = '\0';
	if (!parser_store_map_line(data, line))
	{
		free(line);
		return (0);
	}
	return (1);
}

/* Always consumes line: frees it or transfers it to the map grid. */
static int	_parser_process_line(t_parser_data *data, char *line)
{
	char	*trimmed;
	int		ok;

	trimmed = ft_strtrim(line, " \t\r\n");
	if (!trimmed)
	{
		free(line);
		return (0);
	}
	if (data->map_started || _parser_is_map_start(trimmed))
	{
		free(trimmed);
		return (_parser_process_map_line(data, line));
	}
	ok = 1;
	if (*trimmed)
		ok = parser_parse_config(data, trimmed);
	free(trimmed);
	free(line);
	return (ok);
}

int	parser_read_file(t_parser_data *data, const char *path)
{
	int		fd;
	char	*line;
	int		ok;

	if (!data || !path || ft_strlen(path) < 4 || ft_strncmp(path
			+ ft_strlen(path) - 4, ".cub", 4))
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
