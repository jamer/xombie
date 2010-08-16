#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_

/*
 * A position and direction.
 */

#include "rotation.h"
#include "vector.h"

class Orientation
{
public:
	Orientation();
	Orientation(const Vector location, const Angle angle);
	
	Angle getAngle() const;
	Vector getLocation() const;

	void setAngle(const Angle angle);
	void setLocation(const Vector location);
	void setLocation(const real x, const real y);

	/*
	 * Not rotated.
	 */
	void translate(const Vector coords);
	void translate(const real x, const real y);

	/*
	 * Anything added to an Orientation is transformed by the rotation first.
	 */
	Orientation& operator += (const real distance);
	Orientation& operator += (const Vector coords);

private:
	Vector location;
	Rotation rotation;
};

Orientation operator + (const Orientation& o, real distance);
Orientation operator + (const Orientation& o, const Vector& v);

#endif

