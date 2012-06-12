#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "random.h"

void InitRand()
{
	srand(time(NULL));
}

int randInt(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

real randReal()
{
	return (real)rand() / (real)RAND_MAX;
}

angle randAngle()
{
	return (angle)rand() / (angle)RAND_MAX * M_PI * 2;
}
