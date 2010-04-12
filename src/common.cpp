#include <SDL.h>
#include <sys/stat.h>

#include "common.h"
#include "e.h"

double max(double i1, double i2)
{
	return (i1 < i2 ? i2 : i1);
}

double min(double i1, double i2)
{
	return (i1 > i2 ? i2 : i1);
}

int max(int i1, int i2)
{
	return (i1 < i2 ? i2 : i1);
}

int min(int i1, int i2)
{
	return (i1 > i2 ? i2 : i1);
}

bool startsWith(const char* s1, const char* s2)
{
	while (*s1++ == *s2++)
		if (*s1 == '\0')
			return true;
	return false;
}



inline bool intersect(SDL_Rect* rect, int x, int y)
{
	if (rect->x <= x && x < rect->x + rect->w)
	if (rect->y <= y && y < rect->y + rect->h)
		return true;
	return false;
}

bool testCollision(SDL_Rect* first, SDL_Rect* second)
{
	int x = first->x,
	    y = first->y;
	short w = first->w,
	      h = first->h;

	// first's upper left
	if (intersect(second,
		x,
		y))
		return true;

	// first's upper right
	if (intersect(second,
		x + w,
		y))
		return true;

	// first's lower left
	if (intersect(second,
		x,
		y + h))
		return true;

	// first's lower right
	if (intersect(second,
		x + w,
		y + h))
		return true;

	// center of first
	if (intersect(second,
		x + w / 2,
		y + h / 2))
		return true;
	
	return false;
}


bool readLine(FILE* f, char* buffer, int len)
{
	char c = '\0';
	int i = 0;

	if (len <= 1)
		return false;

	unsigned int cnt = fread(&c, 1, 1, f);

	while (c != '\n' && c != '\r') {
		if (cnt != 1) {
			if (feof(f)) {
				buffer[i] = 0;
				return false;
			}
			else {
				err(1, "Read file failed");
			}
		}

		buffer[i++] = c;

		if (--len == 1)
			break;

		cnt = fread(&c, 1, 1, f);
	}

	buffer[i] = 0;
	return true;
}

char* strip(const char* s)
{
	if (s == NULL || s[0] == '\0')
		return strdup(s);

	int beg = 0, end = 0, len = strlen(s);
	const char* str;

	str = s;
	while (*str == ' ' || *str == '\t') {
		beg++;
		str++;
	}

	str = s + len;
	while (*--str == ' ' || *str == '\t')
		end++;

	int sz = len - beg - end;
	char* r = (char*)malloc(sz + 1);
	strncpy(r, s + beg, sz);
	r[sz] = 0;

	return r;
}

char* readFile(const char* fname)
{
	FILE* f = fopen(fname, "r");
	if (!f)	{
		warn("%s", fname);
		return NULL;
	}

	struct stat stats;
	stat(fname, &stats);
	int len = stats.st_size;

	char* buf = new char[len + 1];
	buf[len] = 0;

	int read = fread(buf, len, 1, f);
	if (read != 1) {
		warn("%s", fname);
		delete buf;
		return NULL;
	}

	fclose(f);

	return buf;
}

uint32_t hash(const char* s)
{
	// very simple hash function
	// suggested by someone on Slashdot
	uint32_t i;
	for (i = 0; *s; s++)
		i = i * 31 + *s;

	return i;
}



/***
 * Some sort of write to file function here
 ***
 * From Slashdot
*
 *
 *
 *

size_t written = 0;
int r = write(fd, &data, sizeof(data))
while (r >= 0 && r + written sizeof((data)) {
	written += r;
	r = write(fd, &data, sizeof(data));
}
if (r 0) { // error handling code, at the very least looking at EIO ENOSPC and EPIPE for network sockets
}


 *
 *
 */

