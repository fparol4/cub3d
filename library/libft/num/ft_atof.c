/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:07 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:07 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	_atof_count_digits(const char **str)
{
	size_t	count;

	count = 0;
	while (**str >= '0' && **str <= '9')
	{
		count++;
		(*str)++;
	}
	return (count);
}

static double	_atof_integer(const char **str)
{
	double	value;

	value = 0.0;
	while (**str >= '0' && **str <= '9')
	{
		value = value * 10.0 + (**str - '0');
		(*str)++;
	}
	return (value);
}

static double	_atof_fraction(const char **str)
{
	double	value;
	double	place;

	value = 0.0;
	place = 0.1;
	while (**str >= '0' && **str <= '9')
	{
		value += (**str - '0') * place;
		place /= 10.0;
		(*str)++;
	}
	return (value);
}

int	ft_vatof(const char *str)
{
	size_t	digits;

	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	digits = _atof_count_digits(&str);
	if (*str == '.')
	{
		str++;
		digits += _atof_count_digits(&str);
	}
	return (*str == '\0' && digits > 0);
}

double	ft_atof(const char *str)
{
	double	sign;
	double	value;

	if (!ft_vatof(str))
		return (0.0);
	sign = 1.0;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			sign = -1.0;
	}
	value = _atof_integer(&str);
	if (*str == '.')
	{
		str++;
		value += _atof_fraction(&str);
	}
	return (value * sign);
}
