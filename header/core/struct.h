/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/10/17 14:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
# include "enum.h"

typedef struct s_image
{
	int			bpp;
	int			stride;
	int			endian;
	int			width;
	int			height;
	char		*addr;
	void		*ptr;
}				t_img;

typedef struct s_player
{
	double		pos_x;
	double		pos_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_line
{
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			error;
}				t_line;

typedef struct s_ray_hit
{
	double		x;
	double		y;
	double		distance;
	t_ray_side	side;
}				t_ray_hit;

typedef struct s_ray_state
{
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	double		delta_x;
	double		delta_y;
	double		side_x;
	double		side_y;
	t_ray_side	side;
}				t_ray_state;

typedef struct s_raycast
{
	double		ray_x;
	double		ray_y;
	t_ray_hit	hit;
	t_ray_state	state;
}				t_raycast;

typedef struct s_point
{
	int			x;
	int			y;
}				t_point;

typedef struct s_viewport
{
	int			x;
	int			y;
	int			width;
	int			height;
}				t_viewport;

typedef struct s_column
{
	t_img		*texture;
	int			tex_x;
	int			height;
	int			top;
	int			start;
	int			end;
}				t_column;

typedef struct s_map
{
	int			width;
	int			height;
	int			color_floor;
	int			color_ceiling;
	char		**grid;
	char		*texture_path[TEX_COUNT];
}				t_map;

typedef struct s_graphics
{
	void		*window;
	void		*mlx;
	t_img		frame;
	t_img		textures[TEX_COUNT];
}				t_graphics;

typedef struct s_game_state
{
	int			exit_status;
	t_view		view;
	int			needs_redraw;
	double		last_frame_time;
	int			keys[GK_COUNT];
}				t_game_state;

typedef struct s_game
{
	t_graphics	graphics;
	t_game_state	state;
	t_map		map;
	t_player	player;
}				t_game;

#endif
