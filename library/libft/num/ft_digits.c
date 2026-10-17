/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_digits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:06 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:06 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_digits(size_t v, int size)
{
	size_t	digits;

	if (size < 2)
		return (0);
	digits = 1;
	while (v >= (size_t)size)
	{
		v /= (size_t)size;
		digits++;
	}
	return (digits);
}
