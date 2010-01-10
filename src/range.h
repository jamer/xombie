#ifndef _RANGE_H_
#define _RANGE_H_

class Range
{
	Range(int min, int max);
	Range(const char* str);
	int get();

private:
	int min;
	int max;

	/** parse()
	 * Try to parse a number range from a string. Returns false if invalid string
	 * used.
	 */
	bool parse(const char* s);
};

#endif // _RANGE_

