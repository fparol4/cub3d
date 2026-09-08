#include "../library/mlx/mlx.h"

// ---- DEFINITIONS
#define G_NAME "CUB3D - Raycast"

#define MLX_WIDTH 1024
#define MLX_HEIGHT 768
#define MLX_EXIT 17

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
	double pox_y;
	double dir_x;
	double dir_y;
	double plane_x;
	double plane_y;
} t_player;

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
	t_img screen;
	t_img textures[4];
} t_game;
