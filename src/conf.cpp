#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "conf.h"

using std::map;

Conf* globals;

/**
 * Conf()
 *
 * Initialize, load, and parse a configuration file.
 */
Conf::Conf(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Could not open '%s'", filename);
		Quit(1);
	}

	char buf[512];
	char* line;
	int sectionh = hash(""), keyh;

	while (readLine(f, buf, 512)) {

		char* comment = strchr(buf, '#');
		if (comment)
			*comment = '\0';

		line = strip(buf);

		if (line[0] == '\0'); // blank line

		else if (line[0] == '[') { // new section
			for (unsigned int i = 0; i < strlen(line); i++)
				line[i] = tolower(line[i]);
			// remove ']'
			line[strlen(line) - 1] = 0;
			// get hash ignoring '['
			sectionh = hash(line + 1);
		}

		else { // new pair
			char* eq = strchr(line, '=');
			*eq = 0;

			char* key = strip(line);
			for (unsigned int i = 0; i < strlen(key); i++)
				key[i] = tolower(key[i]);

			keyh = hash(key);
			free(key);

			char* val = strip(eq + 1);

			data[sectionh][keyh] = val;
		}
	}

	fclose(f);
}


/**
 * getString()
 *
 * Search the configuration for a value.
 * Section and key are case-insensative.
 */
const char* Conf::getString(const char* s, const char* k,
	       	const char* def)
{
	if (s == NULL || strlen(s) == 0 ||
	    k == NULL || strlen(k) == 0)
		return NULL;

	// lower case
	char* s2 = strdup(s);
	for (unsigned int i = 0; i < strlen(s2); i++)
		s2[i] = tolower(s2[i]);
	char* k2 = strdup(k);
	for (unsigned int i = 0; i < strlen(k2); i++)
		k2[i] = tolower(k2[i]);

	// hash
	int h1 = hash(s2);
	int h2 = hash(k2);

	free(s2);
	free(k2);

	// get section and then pair
	map<int, map<int, char*> >::iterator i1 = data.find(h1);
	if (i1 == data.end())
		return def;
	map<int, char*>::iterator i2 = i1->second.find(h2);
	if (i2 == i1->second.end())
		return def;

	const char* str = i2->second;

	return str;
}

/**
 * getInt()
 *
 * Searches the configuration for a value and parses it as an integer.
 * Section and key are case-insensative.
 */
int Conf::getInt(const char* section, const char* key, int def)
{
	const char* str;
	int i = def;

	str = getString(section, key);
	if (str == NULL)
		return def;

	i = atoi(str);

	return i;
}

/**
 * getBool()
 *
 * Searches the configuration for a value and parses it as a boolean.
 * Section and key are case-insensative.
 */
bool Conf::getBool(const char* section, const char* key, bool def)
{
	const char* str;
	bool b = def;

	str = getString(section, key);
	if (str == NULL)
		return def;

	if (!strcmp(str, "true"))
		b = true;
	else if (!strcmp(str, "on"))
		b = true;
	else if (!strcmp(str, "1"))
		b = true;
	else if (!strcmp(str, "enabled"))
		b = true;

	else if (!strcmp(str, "false"))
		b = false;
	else if (!strcmp(str, "off"))
		b = false;
	else if (!strcmp(str, "0"))
		b = false;
	else if (!strcmp(str, "disabled"))
		b = false;

	return b;
}

