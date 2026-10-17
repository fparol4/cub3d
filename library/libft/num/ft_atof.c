/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:18 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:18 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

double	ft_vatof(char *s)
{
	double	dot;
	double	dig;

	if (!s || !*s)
		return (0);
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
	{
		if (ft_isdigit(*s))
			dig = 1;
		else if (*s == '.')
		{
			if (dot)
				return (0);
			else
				dot = 1;
		}
		else
			return (0);
		s++;
	}
	return (dig);
}

double	ft_atof(char *s)
{
	double	sig;
	double	val;
	double	dec;

	val = 0.0;
	dec = 0.1;
	sig = 1.0;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sig = -1;
	while (ft_isdigit(*s))
		val = val * 10 + (*s++ - '0');
	if (*s++ == '.')
	{
		while (ft_isdigit(*s))
		{
			val += (*s++ - '0') * dec;
			dec /= 10.0;
		}
	}
	return (val * sig);
}
