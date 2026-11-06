/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line_reader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"
#include <errno.h>

static int	_parser_read_byte(t_parser_reader *reader, char *byte)
{
	if (reader->offset == (size_t)reader->size)
	{
		reader->size = read(reader->fd, reader->buffer, sizeof(reader->buffer));
		while (reader->size < 0 && errno == EINTR)
			reader->size = read(reader->fd, reader->buffer,
					sizeof(reader->buffer));
		reader->offset = 0;
		if (reader->size < 0)
			return (-1);
		if (!reader->size)
			return (0);
	}
	*byte = reader->buffer[reader->offset++];
	return (1);
}

static int	_parser_append_byte(t_parser_line *line, char byte)
{
	char	*text;
	size_t	capacity;

	if (line->length == line->capacity)
	{
		if (line->capacity > SIZE_MAX / 2)
			return (0);
		capacity = 128;
		if (line->capacity)
			capacity = line->capacity * 2;
		text = malloc(capacity);
		if (!text)
			return (0);
		if (line->length)
			ft_memcpy(text, line->text, line->length);
		free(line->text);
		line->text = text;
		line->capacity = capacity;
	}
	line->text[line->length++] = byte;
	return (1);
}

static int	_parser_collect_line(t_parser_reader *reader, t_parser_line *line)
{
	char	byte;
	int		status;

	status = _parser_read_byte(reader, &byte);
	while (status == 1 && byte != '\n')
	{
		if (!byte || !_parser_append_byte(line, byte))
			return (-1);
		status = _parser_read_byte(reader, &byte);
	}
	if (status < 0 || (!status && !line->length))
		return (status);
	if (!_parser_append_byte(line, '\0'))
		return (-1);
	return (1);
}

int	parser_next_line(t_parser_reader *reader, char **text)
{
	t_parser_line	line;
	int				status;

	line = (t_parser_line){0};
	*text = NULL;
	status = _parser_collect_line(reader, &line);
	if (status == 1)
		*text = line.text;
	else
		free(line.text);
	return (status);
}
