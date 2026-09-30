#ifndef GAME_H
#define GAME_H

#include "../../cub3d.h"

#define MLX_EVENT_KEY_PRESS 2
#define MLX_EVENT_KEY_RELEASE 3
#define MLX_EVENT_FOCUS_OUT 10
#define MLX_MASK_KEY_PRESS (1L << 0)
#define MLX_MASK_KEY_RELEASE (1L << 1)
#define MLX_MASK_FOCUS_CHANGE (1L << 21)

int hook_key_press(int keycode, t_game *game);
int hook_key_release(int keycode, t_game *game);
int hook_focus_out(t_game *game);
int game_frame(t_game *game);

int set_map(t_game *game);
int set_frame(t_game *game);
int set_mlx(t_game *game);
int set_hook(t_game *game);

int game_close(t_game *game);
int game_cleanup(t_game *game);
int game_setup(t_game *game);

int render_px(t_img *img, t_point p, int color);
int render_sqr(t_img *img, t_point point, int size, int color);
void render_line(t_img *img, t_point from, t_point to, int color);
int render_map(t_game *game);
int render_player(t_game *game);
int render_fov(t_game *game);
int update_player(t_game *game, double elapsed);
int cast_ray(t_game *game, double ray_x, double ray_y, t_ray_hit *hit);
#endif
