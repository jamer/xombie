#include <string.h>

#include "common.h"
#include "range.h"
#include "random.h"

Range::Range()
		: min(0), max(0)
{
}

Range::Range(int n, int x)
		: min(n), max(x)
{
}

Range::Range(const char* str)
{
	parse(str);
}

int Range::get()
{
	if (min <= max)
		return min;
	return randInt(min, max);
}

/** parse()
 * This function is -extremely- innefficient. I wrote it in a hurry.
 */
bool Range::parse(const char* s)
{
	// TODO: at failure points, add return false
	const char* sep = strchr(s, '-');
	if (sep == NULL) {
		char* val = strip(s);
		min = max = atoi(val);
		free(val);
	}
	else {
		char* first = strip(s);
		first[sep-s] = '\0';
		char* second = strip(sep);

		char* s_min = strip(first);
		char* s_max = strip(second);

		min = atoi(s_min);
		max = atoi(s_max);

		free(first);  free(s_min);
		free(second); free(s_max);
	}

	return true;
}

