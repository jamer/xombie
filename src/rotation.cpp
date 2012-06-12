#include <math.h>

#include "rotation.h"

Rotation::Rotation()
{
}

Rotation::Rotation(const angle theta)
{
	setAngle(theta);
}

angle Rotation::getAngle() const
{
	return theta;
}

void Rotation::setAngle(const angle theta)
{
	this->theta = theta;
	angle rightAngle = theta + M_PI_2;

	dx = Point(cos(theta), sin(theta));
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

Rotation::operator angle() const
{
	return theta;
}

angle Rotation::operator = (const angle theta)
{
	setAngle(theta);
	return theta;
}

