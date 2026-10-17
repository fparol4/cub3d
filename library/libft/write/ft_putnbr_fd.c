/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:14:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

ssize_t	ft_putnbr_fd(int n, int fd)
{
	char		buffer[12];
	size_t		index;
	long long	value;
	int			sign;

	value = n;
	sign = (value < 0);
	if (sign)
		value = -value;
	index = sizeof(buffer) - 1;
	buffer[index] = '\0';
	if (value == 0)
		buffer[--index] = '0';
	while (value)
	{
		buffer[--index] = (value % 10) + '0';
		value /= 10;
	}
	if (sign)
		buffer[--index] = '-';
	return (ft_write_all(fd, buffer + index, sizeof(buffer) - index - 1));
}
