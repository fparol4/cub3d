/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 16:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 16:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/enum.h"
#include "libft.h"
#include "source/map.h"
#include <stdlib.h>

static unsigned int	_map_texture_field(const char *line)
{
	if (!ft_strncmp(line, "NO", 2))
		return (CUB_NO);
	if (!ft_strncmp(line, "SO", 2))
		return (CUB_SO);
	if (!ft_strncmp(line, "WE", 2))
		return (CUB_WE);
	if (!ft_strncmp(line, "EA", 2))
		return (CUB_EA);
	return (0);
}

static char	**_map_texture_slot(t_map_data *data, unsigned int field)
{
	if (field == CUB_NO)
		return (&data->map.no_texture);
	if (field == CUB_SO)
		return (&data->map.so_texture);
	if (field == CUB_WE)
		return (&data->map.we_texture);
	if (field == CUB_EA)
		return (&data->map.ea_texture);
	return (NULL);
}

static int	_map_parse_texture(t_map_data *data, char *line)
{
	unsigned int	field;
	char			**slot;
	char			*value;

	field = _map_texture_field(line);
	if (!field || (line[2] != ' ' && line[2] != '\t'))
		return (0);
	if (data->fields & field)
		return (0);
	slot = _map_texture_slot(data, field);
	value = ft_strtrim(line + 2, " \t\r\n");
	if (!slot || !value || !*value)
		return (free(value), 0);
	*slot = value;
	data->fields |= field;
	return (1);
}

static int	_map_parse_color_config(t_map_data *data, char *line)
{
	unsigned int	field;
	int				*destination;

	field = CUB_FL;
	destination = &data->map.color_floor;
	if (line[0] == 'C')
	{
		field = CUB_CL;
		destination = &data->map.color_ceiling;
	}
	if ((line[1] != ' ' && line[1] != '\t') || (data->fields & field))
		return (0);
	if (!map_parse_color(line + 2, destination))
		return (0);
	data->fields |= field;
	return (1);
}

int	map_parse_config(t_map_data *data, char *line)
{
	if (!data || !line || !*line)
		return (0);
	if (_map_texture_field(line))
		return (_map_parse_texture(data, line));
	if (line[0] == 'F' || line[0] == 'C')
		return (_map_parse_color_config(data, line));
	return (0);
}
