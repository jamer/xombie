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
	Rotation(const angle theta);

	angle getAngle() const;
	void setAngle(const angle theta);

	Point transform(const real distance) const;
	Point transform(const Point coords) const;

	operator angle() const;
	angle operator = (const angle theta);

private:
	Point dx, dy;
	angle theta;
};

#endif
