#ifndef CUB3D_H
#define CUB3D_H

#include "library/mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>


// ---- DEFINITIONS
#define G_NAME "CUB3D - Raycast"

#define MLX_WIDTH 1024
#define MLX_HEIGHT 768
#define MLX_EXIT 17

#define SIZE_TILE 32
#define SIZE_PLAYER 8

#define COLOR_WALL    0x00808080
#define COLOR_FLOOR   0x00202020
#define COLOR_PLAYER  0x0000FF00

#define TILE_WALL '1'

#define K_ESC 65307

// ---- STRUCTS
typedef struct s_img
{
	int	bpp;
	int llen;
	int endian;
	int width;
	int height;
	char *addr;
	void *ptr;
} t_img;

typedef struct s_player
{
	double pos_x;
	double pos_y;
	double dir_x;
	double dir_y;
	double plane_x;
	double plane_y;
} t_player;

typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_map
{
	int width;
	int height;
	int floor_hx;
	int ceiling_hx;
	char **grid;
	char *no_texture;
	char *so_texture;
	char *we_texture;
	char *ea_texture;
} t_map;

typedef struct s_game
{
	void *mlx;
	void *win;
	t_map map;
	t_img frame;
	t_img texture[4];
	t_player player;
} t_game;

#endif
