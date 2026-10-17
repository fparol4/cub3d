/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*p_1;
	unsigned char	*p_2;

	p_1 = (unsigned char *)s1;
	p_2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p_1[i] != p_2[i])
			return (p_1[i] - p_2[i]);
		i++;
	}
	return (0);
}
