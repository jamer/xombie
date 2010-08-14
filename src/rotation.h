#ifndef _ROTATION_H_
#define _ROTATION_H_

/*
 * Rotates coordinates about the origin in a 2-dimensinal plane. A utility class.
 */

#include "vector.h"

class Rotation
{
public:
	Rotation();
	Rotation(const Angle angle);

	Angle getAngle() const;
	void setAngle(const Angle angle);

	Vector transform(const real distance) const;
	Vector transform(const Vector coords) const;

	operator Angle() const;
	Angle operator = (const Angle angle);

private:
	Vector dx, dy;
	Angle angle;
};

#endif

