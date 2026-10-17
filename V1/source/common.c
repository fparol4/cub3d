#include <time.h>
#include "source/common.h"
#include "core/enum.h"
#include "core/const.h"

int common_kid(int keycode)
{
	if (keycode == K_A)
		return (GK_LEFT);
	if (keycode == K_D)
		return (GK_RIGHT);
	if (keycode == K_LEFT)
		return (GK_ACLOCKWISE);
	if (keycode == K_RIGHT)
		return (GK_CLOCKWISE);
	if (keycode == K_M)
		return (GK_VIEW);
	if (keycode == K_W || keycode == K_UP)
		return (GK_FORWARD);
	if (keycode == K_S || keycode == K_DOWN)
		return (GK_BACKWARD);
	return (-1);
}

double common_now(void)
{
	struct timespec time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return ((double)time.tv_sec + (double)time.tv_nsec / 1000000000.0);
}
