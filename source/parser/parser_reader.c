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

static int	_parser_read_lines(t_parser_data *data, int fd)
{
	t_parser_reader	reader;
	char			*line;
	int				status;

	reader = (t_parser_reader){0};
	reader.fd = fd;
	status = parser_next_line(&reader, &line);
	while (status == 1)
	{
		if (!parser_process_line(data, line))
			return (0);
		status = parser_next_line(&reader, &line);
	}
	return (status == 0);
}

int	parser_read_file(t_parser_data *data, const char *path)
{
	int	fd;
	int	ok;

	if (!data || !path || ft_strlen(path) < 4 || ft_strncmp(path
			+ ft_strlen(path) - 4, ".cub", 4))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	ok = _parser_read_lines(data, fd);
	if (close(fd) < 0)
		ok = 0;
	if (!ok || !data->map.height || !data->map.width)
		return (0);
	return (parser_rectangularize(&data->map));
}
