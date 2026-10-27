/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

int	common_key_id(int keycode)
{
	if (keycode == KEY_A)
		return (GK_LEFT);
	if (keycode == KEY_D)
		return (GK_RIGHT);
	if (keycode == KEY_LEFT)
		return (GK_ACLOCKWISE);
	if (keycode == KEY_RIGHT)
		return (GK_CLOCKWISE);
	if (keycode == KEY_M)
		return (GK_VIEW);
	if (keycode == KEY_W || keycode == KEY_UP)
		return (GK_FORWARD);
	if (keycode == KEY_S || keycode == KEY_DOWN)
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
