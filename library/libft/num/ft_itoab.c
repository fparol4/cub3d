/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stddef.h>

char	*ft_itoab(int v, char *base)
{
	int		b_size;
	int		signal;
	int		digits;
	char	*v_str;

	if (base == NULL)
		return (NULL);
	if (v == 0)
		return (ft_strdup("0"));
	b_size = ft_strlen(base);
	signal = ft_signal(v);
	digits = ft_digits(v, b_size);
	if (digits == 0)
		return (NULL);
	if (signal == -1)
		digits++;
	v_str = ft_calloc((digits + 1), sizeof(char));
	if (signal == -1)
		v_str[0] = '-';
	while (v != 0)
	{
		v_str[--digits] = base[ft_abs(v % b_size) % b_size];
		v = v / b_size;
	}
	return (v_str);
}

char	*ft_itoab_uns(size_t v, char *base)
{
	int		b_size;
	int		digits;
	char	*v_str;

	if (base == NULL)
		return (NULL);
	if (v == 0)
		return (ft_strdup("0"));
	b_size = ft_strlen(base);
	digits = ft_digits(v, b_size);
	if (digits == 0)
		return (NULL);
	v_str = ft_calloc((digits + 1), sizeof(char));
	while (v != 0)
	{
		v_str[--digits] = base[ft_abs(v % b_size) % b_size];
		v = v / b_size;
	}
	return (v_str);
}
