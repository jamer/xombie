#ifndef _ROTATION_H_
#define _ROTATION_H_

/*
 * Rotates coordinates about the origin in a 2-dimensinal plane. A utility class.
 */

#include "point.h"
#include "types.h"

class Rotation
{
public:
	Rotation();
	Rotation(const Angle angle);

	Angle getAngle() const;
	void setAngle(const Angle angle);

	Point transform(const real distance) const;
	Point transform(const Point coords) const;

	operator Angle() const;
	Angle operator = (const Angle angle);

private:
	Point dx, dy;
	Angle angle;
};

#endif

