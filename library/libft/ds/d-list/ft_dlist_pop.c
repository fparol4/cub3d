/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_pop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:05 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:05 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static void	_dlist_reindex(t_dlist_node *node)
{
	int	i;

	i = 0;
	while (node)
	{
		node->index = i++;
		node = node->next;
	}
}

t_dlist_node	*ft_dlist_pop_head(t_dlist *list)
{
	t_dlist_node	*node;

	if (!list || !list->head)
		return (NULL);
	node = list->head;
	list->head = node->next;
	if (list->head)
		list->head->previous = NULL;
	else
		list->tail = NULL;
	list->size--;
	node->next = NULL;
	node->previous = NULL;
	_dlist_reindex(list->head);
	return (node);
}

t_dlist_node	*ft_dlist_pop_tail(t_dlist *list)
{
	t_dlist_node	*node;

	if (!list || !list->tail)
		return (NULL);
	node = list->tail;
	list->tail = node->previous;
	if (list->tail)
		list->tail->next = NULL;
	else
		list->head = NULL;
	list->size--;
	node->next = NULL;
	node->previous = NULL;
	return (node);
}
