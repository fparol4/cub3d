/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/const.h"
#include "core/enum.h"
#include "libft.h"
#include "source/common.h"
#include <stddef.h>
#include <sys/time.h>

int	common_kid(int keycode)
{
	if (keycode == K_A)
		return (GK_LEFT);
	if (keycode == K_D)
		return (GK_RIGHT);
	if (keycode == K_LEFT)
		return (GK_ACLOCKWISE);
	if (keycode == K_RIGHT)
		return (GK_CLOCKWISE);
	if (keycode == K_M)
		return (GK_VIEW);
	if (keycode == K_W || keycode == K_UP)
		return (GK_FORWARD);
	if (keycode == K_S || keycode == K_DOWN)
		return (GK_BACKWARD);
	return (-1);
}

double	common_now(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return (-1.0);
	return ((double)time.tv_sec + (double)time.tv_usec / 1000000.0);
}

int	common_error(char *message)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(message, 2);
	return (1);
}
