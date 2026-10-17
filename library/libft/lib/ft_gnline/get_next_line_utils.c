/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:14:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../../libft.h"
#include <stdint.h>
#include <stdlib.h>

static int	_gnl_reserve(t_gnl_state *state, size_t size)
{
	char	*next;
	size_t	capacity;

	if (size > SIZE_MAX - state->length - 1)
		return (1);
	capacity = state->capacity;
	if (!capacity)
		capacity = (size_t)BUFFER_SIZE;
	while (capacity < state->length + size)
	{
		if (capacity > (SIZE_MAX - 1) / 2)
			capacity = state->length + size;
		else
			capacity *= 2;
	}
	next = malloc(capacity + 1);
	if (!next)
		return (1);
	if (state->length)
		ft_memcpy(next, state->data, state->length);
	free(state->data);
	state->data = next;
	state->capacity = capacity;
	return (0);
}

int	gnl_append(t_gnl_state *state, const char *chunk, size_t size)
{
	if (size > SIZE_MAX - state->length - 1)
		return (1);
	if (state->length + size > state->capacity
		&& _gnl_reserve(state, size))
		return (1);
	ft_memcpy(state->data + state->length, chunk, size);
	state->length += size;
	state->data[state->length] = '\0';
	return (0);
}

size_t	gnl_line_size(t_gnl_state *state)
{
	size_t	i;

	i = 0;
	while (i < state->length && state->data[i] != '\n')
		i++;
	if (i < state->length)
		return (i + 1);
	return (0);
}

char	*gnl_take_line(t_gnl_state *state, size_t line_size)
{
	char	*line;

	line = malloc(line_size + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, state->data, line_size);
	line[line_size] = '\0';
	state->length -= line_size;
	if (state->length)
		ft_memmove(state->data, state->data + line_size, state->length);
	state->data[state->length] = '\0';
	return (line);
}

void	gnl_remove(t_gnl_state **states, int fd)
{
	t_gnl_state	*prev;
	t_gnl_state	*curr;

	prev = NULL;
	curr = *states;
	while (curr && curr->fd != fd)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return ;
	if (prev)
		prev->next = curr->next;
	else
		*states = curr->next;
	free(curr->data);
	free(curr);
}
