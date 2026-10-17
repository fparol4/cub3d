#include <math.h>
#include "source/raycast.h"

static void raycast_setup(t_game *g, t_raycast *rc)
{
	rc->state.map_x = (int) floor (g->player.pos_x);
	rc->state.map_y = (int) floor (g->player.pos_y);
	rc->state.delta_x = 1e30;
	rc->state.delta_y = 1e30;
	if (rc->ray_x != 0.0)
		rc->state.delta_x = fabs(1.0 / rc->ray_x);
	if (rc->ray_y != 0.0)
		rc->state.delta_y = fabs(1.0 / rc->ray_y);
	rc->state.step_x = 1;
	rc->state.step_y = 1;
	rc->state.side_x = (rc->state.map_x + 1 - g->player.pos_x) * rc->state.delta_x;
	rc->state.side_y = (rc->state.map_y + 1 - g->player.pos_y) * rc->state.delta_y;
	if (rc->ray_x < 0.0)
	{
		rc->state.step_x = -1;
		rc->state.side_x = (g->player.pos_x - rc->state.map_x) * rc->state.delta_x;
	}
	if (rc->ray_y < 0.0)
	{
		rc->state.step_y = -1;
		rc->state.side_y = (g->player.pos_y - rc->state.map_y) * rc->state.delta_y;
	}
}

static void raycast_advance(t_raycast *rc)
{
	if (rc->state.side_x < rc->state.side_y)
	{
		rc->hit.distance = rc->state.side_x;
		rc->state.side_x += rc->state.delta_x;
		rc->state.map_x += rc->state.step_x;
		rc->state.side = RAYSIDE_X;
	}
	else
	{
		rc->hit.distance = rc->state.side_y;
		rc->state.side_y += rc->state.delta_y;
		rc->state.map_y += rc->state.step_y;
		rc->state.side = RAYSIDE_Y;
	}
}

static int raycast_in_boundaries(t_game *g, t_raycast *rc)
{
	return (
		rc->state.map_x >= 0 &&
		rc->state.map_y >= 0 &&
		rc->state.map_x < g->map.width &&
		rc->state.map_y < g->map.height
	);
}

static int raycast_is_hit(t_game *g, t_raycast *rc)
{
	if (g->map.grid[rc->state.map_y][rc->state.map_x] == T_FLOOR)
		return (0);
	rc->hit.hit = 1;
	rc->hit.map_x = rc->state.map_x;
	rc->hit.map_y = rc->state.map_y;
	rc->hit.side = rc->state.side;
	rc->hit.x = g->player.pos_x + rc->ray_x * rc->hit.distance;
	rc->hit.y = g->player.pos_y + rc->ray_y * rc->hit.distance;
	return (1);
}

int raycast_cast(t_game *g, t_raycast *rc)
{
	int steps;

	steps = 0;
	rc->hit = (t_ray_hit){0};
	raycast_setup(g, rc);
	while (steps++ < g->map.width + g->map.height)
	{
		raycast_advance(rc);
		if (!raycast_in_boundaries(g, rc))
			return (0);
		if (raycast_is_hit(g, rc))
			return (1);
	}
	return (0);
}
