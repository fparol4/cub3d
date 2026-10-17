/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:07 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:07 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	_itoab_valid_base(const char *base)
{
	size_t	i;
	size_t	j;

	if (!base || ft_strlen(base) < 2)
		return (0);
	i = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = i + 1;
		while (base[j] && base[i] != base[j])
			j++;
		if (base[j])
			return (0);
		i++;
	}
	return (1);
}

static char	*_itoab_create(unsigned long long value, char *base, int sign)
{
	size_t	base_size;
	size_t	digits;
	size_t	index;
	char	*result;

	if (!_itoab_valid_base(base))
		return (NULL);
	base_size = ft_strlen(base);
	digits = ft_digits((size_t)value, (int)base_size);
	result = ft_calloc(digits + sign + 1, sizeof(char));
	if (!result)
		return (NULL);
	index = digits + sign;
	if (!value)
		result[--index] = base[0];
	while (value)
	{
		result[--index] = base[value % base_size];
		value /= base_size;
	}
	if (sign)
		result[0] = '-';
	return (result);
}

char	*ft_itoab(int value, char *base)
{
	unsigned long long	magnitude;

	if (value < 0)
	{
		magnitude = (unsigned long long)(-(long long)value);
		return (_itoab_create(magnitude, base, 1));
	}
	return (_itoab_create((unsigned long long)value, base, 0));
}

char	*ft_itoab_uns(size_t value, char *base)
{
	return (_itoab_create(value, base, 0));
}
