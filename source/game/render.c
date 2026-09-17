#include "game.h"

int render_px(t_img *img, t_point p, int color)
{
	char *dst;

	if (
		p.x < 0 || p.x >= MLX_WIDTH ||
		p.y < 0 || p.y >= MLX_HEIGHT
	)
		return (1);

	dst = img->addr + (p.y * img->llen + p.x * (img->bpp / 8));
	*(unsigned int *)dst = color;
	return (0);
}

int render_sqr(t_img *img, t_point point, int size, int color)
{
  t_point p;

  p.y = point.y;
  while (p.y < point.y + size)
  {
    p.x = point.x;
    while (p.x < point.x + size)
    {
      render_px(img, p, color);
      p.x++;
    }
    p.y++;
  }
  return (0);
}

int render_map(t_game *game)
{
  int color;
  t_point cell;
  t_point pos;

  cell.y = 0;
  while (cell.y < game->map.height)
  {
    cell.x = 0;
    while (cell.x < game->map.width)
    {
      color = COLOR_FLOOR;
      if (game->map.grid[cell.y][cell.x] == TILE_WALL)
        color = COLOR_WALL;
      pos.x = cell.x * SIZE_TILE;
      pos.y = cell.y * SIZE_TILE;
      render_sqr(&game->frame, pos, SIZE_TILE, color);
      cell.x++;
    }
    cell.y++;
  }
  return (0);
}

int render_player(t_game *game)
{
  t_point p;

  p.x = (int)(game->player.pos_x * SIZE_TILE) - SIZE_PLAYER / 2;
  p.y = (int)(game->player.pos_y * SIZE_TILE) - SIZE_PLAYER / 2;
  render_sqr(&game->frame, p, SIZE_PLAYER, COLOR_PLAYER);
  return (0);
}
