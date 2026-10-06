/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

#define FT_ARRAY_DEFAULT_CAP 32

int	ft_array_init(t_array *array, size_t elem_size)
{
	if (!array || !elem_size)
		return (1);
	array->items = NULL;
	array->length = 0;
	array->capacity = 0;
	array->elem_size = elem_size;
	return (0);
}

void	ft_array_free(t_array *array)
{
	if (!array)
		return ;
	free(array->items);
	array->items = NULL;
	array->length = 0;
	array->capacity = 0;
	array->elem_size = 0;
}

void	*ft_array_get(t_array *array, size_t index)
{
	if (!array || !array->items || index >= array->length)
		return (NULL);
	return ((char *)array->items + (index * array->elem_size));
}
