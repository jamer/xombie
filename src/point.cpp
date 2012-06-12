#include <math.h>

#include "point.h"

Point::Point()
	: x(0), y(0)
{
}

Point::Point(real _x, real _y)
	: x(_x), y(_y)
{
}

real Point::distanceTo(const Point& v)
{
	int b = x - v.x;
	int c = y - v.y;
	return (real)sqrt(b*b + c*c);
}

Point& Point::operator += (const Point& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Point& Point::operator *= (const Point& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

Point& Point::operator += (real distance)
{
	x += distance;
	y += distance;
	return *this;
}

Point& Point::operator *= (real distance)
{
	x *= distance;
	y *= distance;
	return *this;
}

Point operator + (const Point& vec, real distance)
{
	Point v = vec;
	v += distance;
	return v;
}

Point operator + (const Point& lhs, const Point& rhs)
{
	Point v = lhs;
	v += rhs;
	return v;
}

Point operator * (const Point& vec, real distance)
{
	Point v = vec;
	v *= distance;
	return v;
}

Point operator * (const Point& lhs, const Point& rhs)
{
	Point v = lhs;
	v *= rhs;
	return v;
}
