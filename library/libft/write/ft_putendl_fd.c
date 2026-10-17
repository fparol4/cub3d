/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:20 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:20 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_putendl_fd(char *s, int fd)
{
	int	l;

	if (!s)
		return (0);
	l = ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
	return (l + 1);
}
