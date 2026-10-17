/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:07 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:07 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

static long long	_atoll_digits(const char *str, int signal)
{
	unsigned long long	result;
	unsigned long long	limit;
	int					digit;

	limit = (unsigned long long)LLONG_MAX + (signal < 0);
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str++ - '0';
		if (result > (limit - digit) / 10)
		{
			if (signal < 0)
				return (LLONG_MIN);
			return (LLONG_MAX);
		}
		result = result * 10 + digit;
	}
	if (signal < 0 && result == limit)
		return (LLONG_MIN);
	if (signal < 0)
		return (-(long long)result);
	return ((long long)result);
}

long long	ft_atoll(const char *str)
{
	int	signal;

	if (!str)
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	signal = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			signal = -1;
	}
	return (_atoll_digits(str, signal));
}
