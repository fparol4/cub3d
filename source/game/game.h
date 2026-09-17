#ifndef GAME_H
#define GAME_H

#include "../../cub3d.h"

int hook_key(int keycode, t_game *game);

int set_map(t_game *game);
int set_frame(t_game *game);
int set_mlx(t_game *game);
int set_hook(t_game *game);

int game_close(t_game *game);
int game_cleanup(t_game *game);
int game_setup(t_game *game);

int render_px(t_img *img, t_point p, int color);
int render_sqr(t_img *img, t_point point, int size, int color);
int render_map(t_game *game);
int render_player(t_game *game);
#endif
