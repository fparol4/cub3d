#ifndef ENUM_H
#define ENUM_H

  typedef enum e_tile
  {
    T_FLOOR = 0,
    T_WALL = 1
  } t_tile;

  typedef enum e_key
  {
    K_FORWARD,
    K_BACKWARD,
    K_LEFT,
    K_RIGHT,
    K_CLOCKWISE,
    K_ACLOCKWISE,
    K_VIEW,
    K_COUNT,
  } e_key;

  typedef enum e_view
  {
    V_3D,
    V_MINIMAP
  } t_view;

  typedef enum e_ray_side
  {
    RAYSIDE_X,
    RAYSIDE_Y
  } t_ray_side;

  typedef enum e_cub_field
  {
    CUB_NO = 1u << 0,
    CUB_SO = 1u << 1,
    CUB_WE = 1u << 2,
    CUB_EA = 1u << 3,
    CUB_FL = 1u << 4,
    CUB_CL = 1u << 5
  } t_cub_field;

  #define CUB_REQ_FIELDS ((1u << 6)) - 1

#endif
