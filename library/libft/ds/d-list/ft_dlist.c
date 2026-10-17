/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dlist	*ft_dlist_new(int id, t_dlist_node *node)
{
	t_dlist			*list;
	t_dlist_node	*curr;

	list = malloc(sizeof(t_dlist));
	if (!list)
		return (NULL);
	list->id = id;
	list->head = node;
	list->tail = NULL;
	list->size = 0;
	curr = node;
	while (curr)
	{
		curr->index = (int)list->size;
		list->tail = curr;
		list->size++;
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
