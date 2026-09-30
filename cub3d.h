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

#define SIZE_TILE 128
#define SIZE_PLAYER 24

#define COLOR_WALL    0x00808080
#define COLOR_FLOOR   0x00202020
#define COLOR_PLAYER  0x0000FF00

#define K_ESC 65307
#define K_W 119
#define K_A 97
#define K_S 115
#define K_D 100
#define K_LEFT 65361
#define K_UP 65362
#define K_RIGHT 65363
#define K_DOWN 65364

typedef enum e_tile
{
	TILE_FLOOR = '0',
	TILE_WALL = '1'
} t_tile;

typedef enum e_game_key
{
	GAME_KEY_FORWARD,
	GAME_KEY_BACKWARD,
	GAME_KEY_STRAFE_LEFT,
	GAME_KEY_STRAFE_RIGHT,
	GAME_KEY_TURN_LEFT,
	GAME_KEY_TURN_RIGHT,
	GAME_KEY_COUNT
} t_game_key;

typedef enum e_view_mode
{
	VIEW_MINIMAP,
	VIEW_3D
} t_view_mode;

typedef enum e_ray_side
{
	RAY_SIDE_X,
	RAY_SIDE_Y
} t_ray_side;

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

typedef struct s_ray_hit
{
	int hit;
	int map_x;
	int map_y;
	t_ray_side side;
	double x;
	double y;
	double distance;
} t_ray_hit;

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
	int keys[GAME_KEY_COUNT];
	t_view_mode view_mode;
	double last_frame;
} t_game;

#endif
