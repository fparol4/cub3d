/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:14:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

static int	_atoi_digits_valid(const char *str, unsigned long long limit)
{
	unsigned long long	value;
	int					digit;

	value = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		digit = *str++ - '0';
		if (value > (limit - digit) / 10)
			return (0);
		value = value * 10 + digit;
	}
	return (1);
}

int	ft_vatoi(const char *str)
{
	unsigned long long	limit;
	int					signal;

	if (!str || !*str)
		return (0);
	signal = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			signal = -1;
	}
	if (!*str)
		return (0);
	limit = (unsigned long long)INT_MAX + (signal < 0);
	return (_atoi_digits_valid(str, limit));
}

int	ft_atoi(const char *str)
{
	long long	value;

	value = ft_atoll(str);
	if (value > INT_MAX)
		return (INT_MAX);
	if (value < INT_MIN)
		return (INT_MIN);
	return ((int)value);
}
