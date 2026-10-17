/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:05 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:05 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dlist	*ft_dlist_new(int id, t_dlist_node *node)
{
	t_dlist			*list;
	t_dlist_node	*curr;
	t_dlist_node	*previous;

	list = malloc(sizeof(t_dlist));
	if (!list)
		return (NULL);
	list->id = id;
	list->head = node;
	list->tail = NULL;
	list->size = 0;
	curr = node;
	previous = NULL;
	while (curr)
	{
		curr->index = (int)list->size;
		curr->previous = previous;
		list->tail = curr;
		list->size++;
		previous = curr;
		curr = curr->next;
	}
	return (list);
}

t_dlist_node	*ft_dlist_node_new(void *content)
{
	t_dlist_node	*node;

	node = malloc(sizeof(t_dlist_node));
	if (!node)
		return (NULL);
	node->content = content;
	node->index = 0;
	node->next = NULL;
	node->previous = NULL;
	return (node);
}
