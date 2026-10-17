/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:19 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:14:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

ssize_t	ft_putendl_fd(const char *s, int fd)
{
	ssize_t	written;

	if (!s)
		return (0);
	written = ft_putstr_fd(s, fd);
	if (written < 0)
		return (-1);
	if (ft_write_all(fd, "\n", 1) < 0)
		return (-1);
	return (written + 1);
}
