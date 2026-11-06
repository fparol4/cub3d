/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
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

static int	_parser_process_content(t_parser_data *data, char *line,
		char *trimmed)
{
	if (!*trimmed)
	{
		data->map_finished = data->map_started;
		return (1);
	}
	if (data->map_finished)
		return (0);
	if (data->map_started || _parser_is_map_start(trimmed))
	{
		data->map_started = 1;
		if (!parser_store_map_line(data, line))
			return (0);
		return (2);
	}
	return (parser_parse_config(data, trimmed));
}

int	parser_process_line(t_parser_data *data, char *line)
{
	char	*trimmed;
	int		status;

	trimmed = ft_strtrim(line, " \t\r\n");
	status = 0;
	if (trimmed)
		status = _parser_process_content(data, line, trimmed);
	free(trimmed);
	if (status != 2)
		free(line);
	return (status != 0);
}
