/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

#define FT_ARRAY_DEFAULT_CAP 32

static int	ft_array_grow(t_array *array)
{
	void	*next;
	size_t	capacity;

	capacity = FT_ARRAY_DEFAULT_CAP;
	if (array->capacity)
		capacity = array->capacity * 2;
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

void	*ft_array_append(t_array *array, void *elem)
{
	void	*slot;

	if (!array || !array->elem_size)
		return (NULL);
	if (!array->capacity || array->length + 1 >= array->capacity)
	{
		if (ft_array_grow(array))
			return (NULL);
	}
	slot = (char *)array->items + (array->length * array->elem_size);
	if (elem)
		ft_memcpy(slot, elem, array->elem_size);
	else
		ft_bzero(slot, array->elem_size);
	array->length++;
	return (slot);
}
