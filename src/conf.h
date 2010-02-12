#ifndef _CONF_H_
#define _CONF_H_

#include <stdio.h>
#include <map>

/**
 * Configuration files
 * 
 * User configuration options interface (sounds techincal, huh?)
 * 
 *
 * example:
 *
 * # pounds inititate single-line comments
 * [Window]
 * width = 640
 * height = 480
 *
 */

class Conf
{
public:
	Conf(const char* filename);

	const char* getString(const char* section, const char* key,
			const char* def = 0);
	int getInt(const char* section, const char* key, int def = 0);
	bool getBool(const char* section, const char* key, bool def = false);

private:
	std::map<int, std::map<int, char*> > data;
};

extern Conf* globals;

#endif //  _CONF_H_

