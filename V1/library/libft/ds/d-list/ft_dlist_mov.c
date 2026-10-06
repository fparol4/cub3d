/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_mov.c                                     :+:      :+:    :+:   */
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

void	ft_dlist_rot(t_dlist *list)
{
	if (!list || list->size < 2)
		return ;
	list->tail->next = list->head;
	list->head->previous = list->tail;
	list->head = list->head->next;
	list->tail = list->tail->next;
	list->head->previous = NULL;
	list->tail->next = NULL;
	ft_dlist_reindex(list->head);
}

void	ft_dlist_revrot(t_dlist *list)
{
	if (!list || list->size < 2)
		return ;
	list->tail->next = list->head;
	list->head->previous = list->tail;
	list->tail = list->tail->previous;
	list->head = list->head->previous;
	list->tail->next = NULL;
	list->head->previous = NULL;
	ft_dlist_reindex(list->head);
}

void	ft_dlist_swap(t_dlist *list)
{
	t_dlist_node	*first;
	t_dlist_node	*second;

	if (!list || list->size < 2)
		return ;
	first = list->head;
	second = first->next;
	first->next = second->next;
	if (second->next)
		second->next->previous = first;
	else
		list->tail = first;
	second->previous = NULL;
	second->next = first;
	first->previous = second;
	list->head = second;
	second->index = 0;
	first->index = 1;
}
