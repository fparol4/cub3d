#include "../cub3d.h"
#include <sys/time.h>

double game_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec + time.tv_usec / 1000000.0);
}
