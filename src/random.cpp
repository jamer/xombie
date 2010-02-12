#include <stdlib.h>
#include <time.h>

void InitRand()
{
	srand(time(NULL));
}

int randInt(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

double randDouble()
{
	return (double)rand() / (double)RAND_MAX;
}

