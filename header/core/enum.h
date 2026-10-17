/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

typedef enum e_tile
{
	T_FLOOR = '0',
	T_WALL = '1'
}	t_tile;

// GAME-KEY
typedef enum e_gkey
{
	GK_FORWARD,
	GK_BACKWARD,
	GK_LEFT,
	GK_RIGHT,
	GK_CLOCKWISE,
	GK_ACLOCKWISE,
	GK_VIEW,
	GK_COUNT,
}	t_gkey;

typedef enum e_view
{
	V_3D,
	V_MINIMAP
}	t_view;

typedef enum e_ray_side
{
	RAYSIDE_X,
	RAYSIDE_Y
}	t_ray_side;

typedef enum e_texture
{
	TEX_NO,
	TEX_SO,
	TEX_WE,
	TEX_EA,
	TEX_COUNT
}	t_texture;

typedef enum e_cub_field
{
	CUB_NO = 1u << 0,
	CUB_SO = 1u << 1,
	CUB_WE = 1u << 2,
	CUB_EA = 1u << 3,
	CUB_FL = 1u << 4,
	CUB_CL = 1u << 5
}	t_cub_field;

# define CUB_REQ_FIELDS 63

#endif
