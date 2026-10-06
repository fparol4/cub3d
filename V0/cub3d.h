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
#define MLX_EVENT_KEY_PRESS 2
#define MLX_EVENT_KEY_RELEASE 3
#define MLX_EVENT_FOCUS_OUT 10
#define MLX_MASK_KEY_PRESS (1L << 0)
#define MLX_MASK_KEY_RELEASE (1L << 1)
#define MLX_MASK_FOCUS_CHANGE (1L << 21)

#define SIZE_TILE 128
#define SIZE_PLAYER 24

#define COLOR_WALL    0x00808080
#define COLOR_FLOOR   0x00202020
#define COLOR_VOID    0x00000000
#define COLOR_WALL_3D 0x00A0A0A0
#define COLOR_WALL_DARK 0x00686868
#define COLOR_PLAYER  0x0000FF00

#define K_ESC 65307
#define K_W 119
#define K_A 97
#define K_S 115
#define K_D 100
#define K_M 109
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
	GAME_KEY_VIEW,
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

typedef enum e_cub_field
{
	CUB_NO = 1u << 0,
	CUB_SO = 1u << 1,
	CUB_WE = 1u << 2,
	CUB_EA = 1u << 3,
	CUB_FLOOR = 1u << 4,
	CUB_CEILING = 1u << 5
} t_cub_field;

#define CUB_REQUIRED_FIELDS ((1u << 6) - 1)

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

typedef struct s_ray_state
{
	int map_x;
	int map_y;
	int step_x;
	int step_y;
	t_ray_side side;
	double delta_x;
	double delta_y;
	double side_x;
	double side_y;
} t_ray_state;

typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_map
{
	int width;
	int height;
	int floor_color;
	int ceiling_color;
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
	double view_key_release_time;
	double last_frame;
} t_game;

int game_setup(t_game *game, const char *map_path);
int game_cleanup(t_game *game);
int game_close(t_game *game);
int game_frame(t_game *game);
int set_frame(t_game *game);
int set_mlx(t_game *game);
int set_hook(t_game *game);
int parse_cub_file(t_game *game, const char *path);
void map_destroy(t_map *map);
int hook_key_press(int keycode, t_game *game);
int hook_view_press(t_game *game);
int hook_key_release(int keycode, t_game *game);
int hook_focus_out(t_game *game);
int render_px(t_img *img, t_point p, int color);
int render_sqr(t_img *img, t_point point, int size, int color);
void render_line(t_img *img, t_point from, t_point to, int color);
void render_line_step(t_point *point, int dx, int dy, int sx, int sy,
	int *error);
int render_map(t_game *game);
int render_player(t_game *game);
int render_fov(t_game *game);
int render_3d(t_game *game);
void render_3d_column(t_game *game, int x);
void render_wall_column(t_game *game, int x, t_ray_hit *hit);
double render_camera_x(int x, int width);
void render_fov_ray(t_game *game, int i, t_point from);
int render_tile_color(char tile);
void render_map_cell(t_game *game, int x, int y);
int update_player(t_game *game, double elapsed);
void player_move(t_game *game, double forward, double strafe, double elapsed);
int cast_ray(t_game *game, double ray_x, double ray_y, t_ray_hit *hit);
void ray_setup(t_game *game, double ray_x, double ray_y, t_ray_state *state);
void ray_advance(t_ray_state *state, t_ray_hit *hit);
int ray_in_bounds(t_game *game, t_ray_state *state);
int ray_finish_hit(t_game *game, double ray_x, double ray_y,
	t_ray_state *state, t_ray_hit *hit);
double game_time(void);
int input_key_index(int keycode);
int player_cell_blocked(t_game *game, double x, double y);
int player_can_stand(t_game *game, double x, double y);
void player_rotate(t_player *player, double angle);
void render_column(t_img *frame, int x, int start, int end, int wall_color,
	int ceiling_color, int floor_color);

char *cub_trim_line(char *line);
int cub_is_blank(const char *line);
int cub_parse_directive(t_map *map, const char *line, unsigned int *seen);
unsigned int cub_directive_field(const char *key, size_t len, t_map *map,
	char ***texture);
unsigned int cub_texture_field(const char *key, size_t len, t_map *map,
	char ***texture);
int cub_store_directive(t_map *map, unsigned int field, const char *value,
	char **texture);
int cub_process_line(t_game *game, char *line, int *map_started,
	char ***lines, int *count, unsigned int *seen);
int cub_read_lines(t_game *game, int fd, char ***lines, int *count,
	unsigned int *seen);
int cub_finish_parse(t_game *game, char **lines, int count,
	unsigned int seen);
int cub_is_map_line(const char *line);
int cub_append_line(char ***lines, int *count, const char *line);
int cub_map_width(char **lines, int count);
int cub_copy_grid_row(char *destination, const char *source);
int cub_alloc_grid_row(t_game *game, char **lines, int row, int width);
int cub_store_grid(t_game *game, char **lines, int count);
int cub_validate_map(t_game *game);
int cub_parse_color(const char *value, int *color);
int cub_parse_component(const char **value, long *component, int comma);
int cub_find_player(t_game *game);
int cub_cell_enclosed(t_game *game, int x, int y);
int cub_check_enclosure(t_game *game);
void cub_free_lines(char **lines, int count);
int cub_is_walkable(char tile);
int cub_is_spawn(char tile);
int cub_is_valid_tile(char tile);
void cub_set_player(t_game *game, char tile, int x, int y);

#endif
