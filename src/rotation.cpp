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

	dx = Vector(cos(angle), sin(angle));
	dy = Vector(cos(rightAngle), sin(rightAngle));
}

Vector Rotation::transform(const real distance) const
{
	return dx * distance;
}

Vector Rotation::transform(const Vector coords) const
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

