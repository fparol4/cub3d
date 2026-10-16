#include <math.h>
#include "shared/enum.h"
#include "shared/struct.h"

static int _player_cell_valid(t_game *g, double x, double y)
{
	int cell_x;
	int cell_y;

	cell_x = (int) floor(x);
	cell_y = (int) floor(y);
	if (cell_x < 0 || cell_x >= g->map.width)
		return (1);
	if (cell_y < 0 || cell_y >= g->map.height)
		return (1);
	return (g->map.grid[cell_y][cell_x] != T_FLOOR);
}

static int _player_pos_valid(t_game *g, double x, double y)
{
	double rd;

	rd = 0.36;
	return (
		!_player_cell_valid(g, x + rd, y + rd) &&
		!_player_cell_valid(g, x + rd, y - rd) &&
		!_player_cell_valid(g, x - rd, y + rd) &&
		!_player_cell_valid(g, x - rd, y - rd)
	);
}

static int _player_move(t_game *g, double elapsed)
{
	double fw;
	double st;
	double ln;
	double mx;
	double my;

	fw = g->keys[GK_FORWARD] - g->keys[GK_BACKWARD];
	st = g->keys[GK_LEFT] - g->keys[GK_RIGHT];
	ln = sqrt(pow(fw, 2) + pow(st, 2));
	if (ln == 0.0)
		return (0);
	fw /= ln;
	st /= ln;
	mx = (g->player.dir_x * fw - g->player.dir_y * st) * 2.5 * elapsed;
	my = (g->player.dir_y * fw + g->player.dir_x * st) * 2.5 * elapsed;
	if (_player_pos_valid(g, g->player.pos_x + mx, g->player.pos_y))
		g->player.pos_x += mx;
	if (_player_pos_valid(g, g->player.pos_x, g->player.pos_y + my))
		g->player.pos_x += my;
	return (0);
}

static void _player_rotate(t_player *p, double angle)
{
	double ox;
	double cs;
	double sn;

	ox = p->dir_x;
	cs = cos(angle);
	sn = sin(angle);
	// direction
	p->dir_x = p->dir_x * cs - p->dir_y * sn;
	p->dir_y = ox * sn - p->dir_y * cs;
	// plane
	ox = p->plane_x;
	p->plane_x = p->plane_x * cs - p->plane_y * sn;
	p->plane_y = ox * sn - p->plane_y * cs;
}

int player_update(t_game *g, double elapsed)
{
	_player_move(g, elapsed);
	if (g->keys[GK_ACLOCKWISE])
		_player_rotate(&g->player, 1.8 * elapsed);
	if (g->keys[GK_CLOCKWISE])
		_player_rotate(&g->player, -1.8 * elapsed);
	return (0);
}
