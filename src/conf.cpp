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
QString Conf::getString(QString s, QString k, QString def) const
{
	s = s.toLower();
	k = k.toLower();

	// hash
	int h1 = hash(s.toUtf8().data());
	int h2 = hash(k.toUtf8().data());

	// get section and then pair
	QHash<int, QHash<int, char*> >::const_iterator i1 = data.find(h1);
	if (i1 == data.end())
		return def;
	QHash<int, char*> section = i1.value();
	QHash<int, char*>::const_iterator i2 = section.find(h2);
	if (i2 == section.end())
		return def;

	char* str = i2.value();

	return str;
}

const char* Conf::getCString(QString s, QString k, QString def) const
{
	return getString(s, k, def).toUtf8().data();
}

/**
 * getInt()
 *
 * Searches the configuration for a value and parses it as an integer.
 * Section and key are case-insensative.
 */
int Conf::getInt(QString section, QString key, int def) const
{
	bool ok;
	int i = getString(section, key).toInt(&ok);
	if (!ok)
		return def;
	return i;
}

Range Conf::getRange(QString section, QString key, int deflow, int defhigh)
	const
{
	QString s = getString(section, key);
	if (s.isEmpty())
		return Range(deflow, defhigh);
	return Range(s);
}

/**
 * getBool()
 *
 * Searches the configuration for a value and parses it as a boolean.
 * Section and key are case-insensative.
 */
bool Conf::getBool(QString section, QString key, bool def) const
{
	QString s = getString(section, key);
	if (s.isEmpty())
		return def;

	if (s == "true" || s == "on" || s == "1" || s == "enabled")
		return true;
	else if (s == "false" || s == "off" || s == "0" || s == "disabled")
		return false;
	return def;
}

