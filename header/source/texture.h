/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H
# include "core/struct.h"

int		texture_load(t_game *g);
void	texture_cleanup(t_game *g);
t_img	*texture_face(t_game *g, t_raycast *rc);
int		texture_pixel(t_img *img, int x, int y);
#endif
