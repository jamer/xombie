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
	Conf(QString filename);
	~Conf();

	QString getString(QString section, QString key,
			QString def = QString()) const;
	const char* getCString(QString section, QString key,
			QString def = QString()) const;

	int getInt(QString section, QString key, int def = 0) const;
	Range getRange(QString section, QString key,
			int deflow, int defhigh) const;
	bool getBool(QString section, QString key, bool def = false) const;

private:
	QHash<int, QHash<int, char*> > data;

	typedef QHash<int, QHash<int, char*> >::const_iterator section_it;
	typedef QHash<int, char*> section_t;
	typedef QHash<int, char*>::const_iterator key_it;
};

extern Conf* globals;

#endif //  _CONF_H_

