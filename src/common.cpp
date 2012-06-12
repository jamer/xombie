#include <SDL.h>
#include <sys/stat.h>

#include "common.h"
#include "e.h"

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);
QTextStream cerr(stderr, QIODevice::WriteOnly);


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
				err("Read file failed");
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

char* strip(const char* str)
{
	int beg = 0, end = 0, sz, len;
	const char* s;
	char* rem;

	if (str == NULL || str[0] == '\0')
		return strdup(str);

	len = strlen(str);
	for (s = str; *s == ' ' || *s == '\t'; s++, beg++);
	for (s = str + len - 1; *s == ' ' || *s == '\t'; s--, end++);

	sz = len - (beg + end);
	rem = (char*)malloc(sz + 1);
	strncpy(rem, str + beg, sz);
	rem[sz] = 0;
	return rem;
}

char* readFile(const char* fname)
{
	FILE* f;
	struct stat stats;
	char* buf;
	int len, read;

	if (!(f = fopen(fname, "r")))
		goto err;
	if (stat(fname, &stats))
		goto err;

	len = stats.st_size;
	buf = (char*)malloc(len + 1);
	buf[len] = '\0';

	read = fread(buf, len, 1, f);
	fclose(f);

	if (read != 1)
		goto errbuf;
	return buf;

errbuf:
	free(buf);
err:
	warn(fname);
	return NULL;
}

uint32_t hash(const char* s)
{
	uint32_t i;
	for (i = 0; *s; s++)
		i = i * 31 + *s;

	return i;
}
