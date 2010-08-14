#include "orientation.h"

Orientation::Orientation()
{
}

Orientation::Orientation(Vector loc, Angle angle)
	: location(loc), rotation(angle)
{
}

Angle Orientation::getAngle() const
{
	return rotation;
}

Vector Orientation::getLocation() const
{
	return location;
}

void Orientation::setAngle(const Angle angle)
{
	rotation = angle;
}

void Orientation::setLocation(const Vector loc)
{
	location = loc;
}

void Orientation::translate(const Vector coords)
{
	location += coords;
}

Orientation& Orientation::operator += (const real distance)
{
	location += rotation.transform(distance);
	return *this;
}

Orientation& Orientation::operator += (const Vector coords)
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

Orientation operator + (const Orientation& orient, const Vector& v)
{
	Orientation o(orient);
	o += v;
	return o;
}

