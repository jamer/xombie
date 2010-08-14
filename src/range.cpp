#include <QStringList>
#include <stdio.h>

#include "range.h"
#include "random.h"

Range::Range(int a, int b)
		: min(a), max(b)
{
}

Range::Range(QString range)
{
	parse(range);
}

int Range::value()
{
	if (min >= max)
		return min;
	return randInt(min, max);
}

/** parse()
 * Parses a string to find a range.
 */
bool Range::parse(QString range)
{
	QStringList parts = range.split("-");
	bool minOk, maxOk;

	min = parts[0].trimmed().toInt(&minOk);
	max = parts[1].trimmed().toInt(&maxOk);

	if (!minOk || !maxOk) {
		printError(range);
		return false;
	}

	return true;
}

void Range::printError(QString range)
{
	printf("Range::parse() - Error parsing string '%s'\n",
			range.toUtf8().data());
}


