#include "game.h"

int hook_key(int keycode, t_game *game)
{
  if (keycode == K_ESC)
    game_close(game);
  return (0);
}
