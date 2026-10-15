#include "shared/cub3d.h"
#include "source/runtime.h"

int main(int argc, char **argv)
{
	t_game g;

	if (runtime_args(argc, argv))
		return (1);
	if (runtime_setup(&g, argv[1]))
		return (1);
	runtime_run(g.mlx);
	return (0);
}
