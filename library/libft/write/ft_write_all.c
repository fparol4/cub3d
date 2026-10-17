/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 17:31:07 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 17:31:07 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <errno.h>
#include <limits.h>

ssize_t	ft_write_all(int fd, const void *buffer, size_t size)
{
	const char	*data;
	size_t		offset;
	ssize_t		written;

	if ((!buffer && size) || size > SSIZE_MAX)
		return (-1);
	data = (const char *)buffer;
	offset = 0;
	while (offset < size)
	{
		written = write(fd, data + offset, size - offset);
		if (written < 0 && errno == EINTR)
			continue ;
		if (written <= 0)
			return (-1);
		offset += (size_t)written;
	}
	return ((ssize_t)offset);
}
