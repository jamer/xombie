#include <math.h>
#include "orientation.h"

Orientation::Orientation()
	: location(), angle(0)
{
}

Orientation::Orientation(Vector loc, Angle a)
	: location(loc), angle(a)
{
}

Angle Orientation::getAngle() const
{
	return angle;
}

Vector Orientation::getLocation() const
{
	return location;
}

void Orientation::setAngle(Angle a)
{
	angle = a;
	calcUnits();
}

void Orientation::setLocation(Vector loc)
{
	location = loc;
}

void Orientation::translate(Vector coordinates)
{
	location += coordinates;
}

Orientation& Orientation::operator += (real distance)
{
	location += dx * distance;
	return *this;
}

Orientation& Orientation::operator += (Vector v)
{
	location += dx * v.x;
	location += dy * v.y;
	return *this;
}

void Orientation::calcUnits()
{
	Angle rightAngle = angle + M_PI_2;

	dx = Vector(cos(angle), sin(angle));
	dy = Vector(cos(rightAngle), sin(rightAngle));
}

Orientation operator + (const Orientation& orient, real distance)
{
	Orientation o(orient);
	o += distance;
	return o;
}

Orientation operator + (const Orientation& orient, const Vector& v)
{
	Orientation o(orient);
	o += v;
	return o;
}

