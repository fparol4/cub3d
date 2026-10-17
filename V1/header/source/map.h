#ifndef MAP_H
#define MAP_H
	#include "core/struct.h"

	int map_cleanup(t_map *m);
	int map_parse(t_game *g, char *map_path);
#endif
