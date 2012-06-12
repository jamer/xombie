#include <math.h>

#include "point.h"

Point::Point()
	: x(0), y(0)
{
}

Point::Point(real x, real y)
	: x(x), y(y)
{
}

real Point::distanceTo(const Point& p)
{
	int b = x - p.x;
	int c = y - p.y;
	return (real)sqrt(b*b + c*c);
}

Point& Point::operator += (const Point& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

Point& Point::operator *= (const Point& p)
{
	x *= p.x;
	y *= p.y;
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

Point operator + (const Point& pt, real distance)
{
	Point p = pt;
	p += distance;
	return p;
}

Point operator + (const Point& lhs, const Point& rhs)
{
	Point p = lhs;
	p += rhs;
	return p;
}

Point operator * (const Point& pt, real distance)
{
	Point p = pt;
	p *= distance;
	return p;
}

Point operator * (const Point& lhs, const Point& rhs)
{
	Point p = lhs;
	p *= rhs;
	return p;
}
