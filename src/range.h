#ifndef _RANGE_H_
#define _RANGE_H_

#include <QString>

class Range
{
public:
	Range(int min, int max);
	Range(QString s);
	int value();

private:
	int min;
	int max;

	/** parse()
	 * Try to parse a number range from a string. Returns false if invalid
	 * string used.
	 */
	bool parse(QString s);

	/** printError()
	 * Used to print an error if parsing had a problem.
	 */
	void printError(QString range);
};

#endif // _RANGE_

