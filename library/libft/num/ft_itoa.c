/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_itoa(int n)
{
	char	*str;
	size_t	nb;
	size_t	len;
	size_t	i;

	if (n < 0)
		nb = (size_t)(-(long long)n);
	else
		nb = (size_t)n;
	len = ft_digits(nb, 10);
	str = (char *)malloc(len + (n < 0) + 1);
	if (!str)
		return (0);
	i = len + (n < 0);
	str[i] = '\0';
	if (nb == 0)
		str[--i] = '0';
	while (nb)
	{
		str[--i] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
