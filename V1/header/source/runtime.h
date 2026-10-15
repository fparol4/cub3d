#ifndef RUNTIME_H
#define RUNTIME_H
	#include "shared/cub3d.h"

	int runtime_run(t_game *g);
	int runtime_close(t_game *g);
	int runtime_cleanup(t_game *g);
	int runtime_args(int argc, char **argv);
	int runtime_setup(t_game *g, char *map_path);
#endif
