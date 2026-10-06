/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_padstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:19 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "stdio.h"

char	*ft_padstart(char *str, int max_size, char placeholder)
{
	int		i;
	int		len;
	int		diff;
	char	pad[32];
	char	*str_wpad;

	len = ft_strlen(str);
	if (len >= max_size)
		return (str);
	diff = max_size - len;
	if ((size_t)diff > sizeof(pad))
		return (NULL);
	i = 0;
	while (i < diff)
		pad[i++] = placeholder;
	str_wpad = ft_calloc(max_size + 1, sizeof(char));
	ft_memcpy(str_wpad, pad, diff);
	ft_memcpy(str_wpad + diff, str, len);
	return (str_wpad);
}
