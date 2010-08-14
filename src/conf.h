#ifndef _CONF_H_
#define _CONF_H_

#include <QHash>
#include <QString>
#include <stdio.h>

#include "range.h"

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
	~Conf();

	const char* getString(QString section, QString key,
			const char* def = 0);
	int getInt(QString section, QString key, int def = 0);
	Range getRange(QString section, QString key,
			int deflow, int defhigh);
	bool getBool(QString section, QString key, bool def = false);

private:
	QHash<int, QHash<int, char*> > data;
};

extern Conf* globals;

#endif //  _CONF_H_

