/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_add.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static void	ft_dlist_reindex(t_dlist_node *node)
{
	int	i;

	i = 0;
	while (node)
	{
		node->index = i++;
		node = node->next;
	}
}

void	ft_dlist_add_head(t_dlist *list, t_dlist_node *node)
{
	if (!list || !node)
		return ;
	node->next = list->head;
	node->previous = NULL;
	if (list->head)
		list->head->previous = node;
	else
		list->tail = node;
	list->head = node;
	list->size++;
	ft_dlist_reindex(list->head);
}

void	ft_dlist_add_tail(t_dlist *list, t_dlist_node *node)
{
	if (!list || !node)
		return ;
	node->previous = list->tail;
	node->next = NULL;
	node->index = (int)list->size;
	if (list->tail)
		list->tail->next = node;
	else
		list->head = node;
	list->tail = node;
	list->size++;
}
