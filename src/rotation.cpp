#include <math.h>

#include "rotation.h"

Rotation::Rotation()
{
}

Rotation::Rotation(const Angle angle)
{
	setAngle(angle);
}

Angle Rotation::getAngle() const
{
	return angle;
}

void Rotation::setAngle(const Angle a)
{
	angle = a;
	Angle rightAngle = angle + M_PI_2;

	dx = Point(cos(angle), sin(angle));
	dy = Point(cos(rightAngle), sin(rightAngle));
}

Point Rotation::transform(const real distance) const
{
	return dx * distance;
}

Point Rotation::transform(const Point coords) const
{
	return dx * coords.x + dy * coords.y;
}

Rotation::operator Angle() const
{
	return angle;
}

Angle Rotation::operator = (const Angle angle)
{
	setAngle(angle);
	return angle;
}

