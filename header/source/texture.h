/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H
# include "core/struct.h"

int		texture_load(t_game *g);
void	texture_cleanup(t_game *g);
t_img	*texture_for_hit(t_game *g, t_raycast *rc);
#endif
