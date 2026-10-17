/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:05 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:05 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"
#include <stdint.h>

#define FT_ARRAY_DEFAULT_CAP 32

static int	_array_grow(t_array *array)
{
	void	*next;
	size_t	capacity;

	capacity = FT_ARRAY_DEFAULT_CAP;
	if (array->capacity)
	{
		if (array->capacity > SIZE_MAX / 2)
			return (1);
		capacity = array->capacity * 2;
	}
	if (array->elem_size && capacity > SIZE_MAX / array->elem_size)
		return (1);
	next = ft_calloc(capacity, array->elem_size);
	if (!next)
		return (1);
	if (array->items)
		ft_memcpy(next, array->items, array->length * array->elem_size);
	free(array->items);
	array->items = next;
	array->capacity = capacity;
	return (0);
}

static int	_array_stage(t_array *array, void **elem, int grow, void **copy)
{
	size_t		used;
	uintptr_t	source;
	uintptr_t	start;

	*copy = NULL;
	if (!grow || !*elem || !array->items)
		return (0);
	if (array->length > SIZE_MAX / array->elem_size)
		return (1);
	used = array->length * array->elem_size;
	source = (uintptr_t)(*elem);
	start = (uintptr_t)array->items;
	if (source < start || source - start >= used)
		return (0);
	*copy = malloc(array->elem_size);
	if (!*copy)
		return (1);
	ft_memcpy(*copy, *elem, array->elem_size);
	*elem = *copy;
	return (0);
}

void	*ft_array_append(t_array *array, void *elem)
{
	void	*slot;
	void	*copy;
	int		grow;

	if (!array || !array->elem_size)
		return (NULL);
	grow = (!array->capacity || array->length >= array->capacity);
	if (_array_stage(array, &elem, grow, &copy))
		return (NULL);
	if (grow && _array_grow(array))
		return (free(copy), NULL);
	slot = (char *)array->items + (array->length * array->elem_size);
	if (elem)
		ft_memmove(slot, elem, array->elem_size);
	else
		ft_bzero(slot, array->elem_size);
	array->length++;
	free(copy);
	return (slot);
}
