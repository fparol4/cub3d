/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:14:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../../libft.h"
#include <errno.h>

static t_gnl_state	*g_gnl_states;

static t_gnl_state	*_gnl_find_or_create(t_gnl_state **states, int fd)
{
	t_gnl_state	*state;

	state = *states;
	while (state && state->fd != fd)
		state = state->next;
	if (state)
		return (state);
	state = ft_calloc(1, sizeof(*state));
	if (!state)
		return (NULL);
	state->fd = fd;
	state->next = *states;
	*states = state;
	return (state);
}

static int	_gnl_read_chunk(int fd, t_gnl_state *state)
{
	char	*chunk;
	ssize_t	bytes;
	int		result;

	chunk = malloc((size_t)BUFFER_SIZE);
	if (!chunk)
		return (-1);
	bytes = read(fd, chunk, (size_t)BUFFER_SIZE);
	while (bytes < 0 && errno == EINTR)
		bytes = read(fd, chunk, (size_t)BUFFER_SIZE);
	result = 1;
	if (bytes < 0)
		result = -1;
	else if (bytes == 0)
		result = 0;
	else if (gnl_append(state, chunk, (size_t)bytes))
		result = -1;
	free(chunk);
	return (result);
}

static int	_gnl_fill(int fd, t_gnl_state *state)
{
	int	result;

	while (!gnl_line_size(state))
	{
		result = _gnl_read_chunk(fd, state);
		if (result != 1)
			return (result);
	}
	return (1);
}

static char	*_gnl_finish(t_gnl_state **states, t_gnl_state *state, int status)
{
	char	*line;
	size_t	line_size;

	if (!status && !state->length)
		return (NULL);
	line_size = gnl_line_size(state);
	if (!line_size)
		line_size = state->length;
	line = gnl_take_line(state, line_size);
	if (!line || !status)
		gnl_remove(states, state->fd);
	return (line);
}

char	*ft_get_next_line(int fd)
{
	t_gnl_state	*state;
	int			status;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	state = _gnl_find_or_create(&g_gnl_states, fd);
	if (!state)
		return (NULL);
	status = _gnl_fill(fd, state);
	if (status < 0)
		return (gnl_remove(&g_gnl_states, fd), NULL);
	return (_gnl_finish(&g_gnl_states, state, status));
}
