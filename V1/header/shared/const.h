#ifndef CONST_H
#define CONST_H
  #define G_NAME "CUB3D - Raycast"

  #define S_WIDTH 1024
  #define S_HEIGHT 768
  #define S_TILE 96
  #define S_PLAYER 24
  #define PLAYER_MAX_ELAPSED 0.05
  #define RENDER_FOV_RAYS 41
  #define RENDER_MIN_DISTANCE 0.05

  #define EVT_EXIT 17
  #define EVT_KEY_PRESS 2
  #define EVT_KEY_RELEASE 3
  #define EVT_FOCUS_OUT 10
  #define MASK_KEY_PRESS (1L << 0)
  #define MASK_KEY_RELEASE (1L << 1)
  #define MASK_STRUCTURE_NOTIFY (1L << 17)
  #define MASK_FOCUS_CHANGE (1L << 21)

  #define COLOR_WALL    0x00808080
  #define COLOR_FLOOR   0x00202020
  #define COLOR_VOID    0x00000000
  #define COLOR_WALL_3D 0x00A0A0A0
  #define COLOR_WALL_DARK 0x00686868
  #define COLOR_PLAYER  0x0000FF00
  #define COLOR_FOV     0x00D09020

  #define K_W 119
  #define K_A 97
  #define K_S 115
  #define K_D 100
  #define K_M 109
  #define K_UP 65362
  #define K_RIGHT 65363
  #define K_LEFT 65361
  #define K_DOWN 65364
  #define K_ESC 65307
#endif
