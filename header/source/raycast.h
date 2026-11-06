/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H
# include "core/struct.h"

void	raycast_camera_ray(t_game *g, t_raycast *ray, int index);
int		raycast_cast(t_game *g, t_raycast *rc);
#endif
