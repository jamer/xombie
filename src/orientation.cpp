#include "orientation.h"

Orientation::Orientation()
{
}

Orientation::Orientation(Point loc, angle a)
	: location(loc), rotation(a)
{
}

angle Orientation::getAngle() const
{
	return rotation;
}

Point Orientation::getLocation() const
{
	return location;
}

void Orientation::setAngle(const angle a)
{
	rotation = a;
}

void Orientation::setLocation(const Point loc)
{
	location = loc;
}

void Orientation::setLocation(const real x, const real y)
{
	location.x = x;
	location.y = y;
}

void Orientation::translate(const Point coords)
{
	location += coords;
}

void Orientation::translate(const real x, const real y)
{
	location.x += x;
	location.y += y;
}

Orientation& Orientation::operator += (const real distance)
{
	location += rotation.transform(distance);
	return *this;
}

Orientation& Orientation::operator += (const Point coords)
{
	location += rotation.transform(coords);
	return *this;
}

Orientation operator + (const Orientation& orient, real distance)
{
	Orientation o(orient);
	o += distance;
	return o;
}

Orientation operator + (const Orientation& orient, const Point& v)
{
	Orientation o(orient);
	o += v;
	return o;
}

