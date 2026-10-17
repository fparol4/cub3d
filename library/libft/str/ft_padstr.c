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
#include <stdint.h>

char	*ft_padstart(const char *str, size_t max_size, char placeholder)
{
	size_t	i;
	size_t	len;
	size_t	diff;
	char	*str_wpad;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < max_size)
		diff = max_size - len;
	else
		diff = 0;
	if (diff > SIZE_MAX - len - 1)
		return (NULL);
	str_wpad = ft_calloc(len + diff + 1, sizeof(char));
	if (!str_wpad)
		return (NULL);
	i = 0;
	while (i < diff)
		str_wpad[i++] = placeholder;
	ft_memcpy(str_wpad + diff, str, len + 1);
	return (str_wpad);
}
