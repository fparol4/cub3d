/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H
# include "core/struct.h"

void	render_3d(t_game *g);
void	render_map_view(t_game *g, t_viewport *v);
void	render_fov(t_game *g, t_viewport *v);
void	render_px(t_img *img, t_point p, int color);
void	render_line(t_img *img, t_point from, t_point to, int color);
void	render_rect(t_img *img, t_point from, t_point to, int color);
t_point	render_world_to_map(t_game *g, t_viewport *v, double x, double y);
#endif
