#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "conf.h"
#include "engine.h"

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
		
		free(line);
	}

	fclose(f);
}

Conf::~Conf()
{
}


/**
 * getString()
 *
 * Search the configuration for a value.
 * Section and key are case-insensative.
 */
const char* Conf::getString(QString s, QString k,
	       	const char* def)
{
	s = s.toLower();
	k = k.toLower();

	// hash
	int h1 = hash(s.toUtf8().data());
	int h2 = hash(k.toUtf8().data());

	// get section and then pair
	QHash<int, QHash<int, char*> >::iterator i1 = data.find(h1);
	if (i1 == data.end())
		return def;
	QHash<int, char*> section = i1.value();
	QHash<int, char*>::iterator i2 = section.find(h2);
	if (i2 == section.end())
		return def;

	char* str = i2.value();

	return str;
}

/**
 * getInt()
 *
 * Searches the configuration for a value and parses it as an integer.
 * Section and key are case-insensative.
 */
int Conf::getInt(QString section, QString key, int def)
{
	const char* str;
	int i = def;

	str = getString(section, key);
	if (str == NULL)
		return def;

	i = atoi(str);

	return i;
}

Range Conf::getRange(QString section, QString key, int deflow, int defhigh)
{
	const char* str;
	Range def(deflow, defhigh);

	str = getString(section, key);
	if (str == NULL)
		return def;

	Range r(str);

	return r;
}

/**
 * getBool()
 *
 * Searches the configuration for a value and parses it as a boolean.
 * Section and key are case-insensative.
 */
bool Conf::getBool(QString section, QString key, bool def)
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

