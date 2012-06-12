#ifndef _POINT_H_
#define _POINT_H_

#include "types.h"

/*
 * An X-Y location.
 */
class Point
{
public:
	Point();
	Point(real x, real y);

	real distanceTo(const Point& v);

	Point& operator += (const Point& v);
	Point& operator *= (const Point& v);

	Point& operator += (real distance);
	Point& operator *= (real distance);

	real x, y;
};

Point operator + (const Point& v, real distance);
Point operator + (const Point& lhs, const Point& rhs);

Point operator * (const Point& v, real distance);
Point operator * (const Point& lhs, const Point& rhs);

#endif
