#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_

/*
 * A position and direction.
 */

#include "vector.h"

class Orientation
{
public:
	Orientation();
	Orientation(Vector location, Angle angle);
	
	Angle getAngle() const;
	Vector getLocation() const;

	void setAngle(Angle angle);
	void setLocation(Vector location);

	/*
	 * Not rotated.
	 */
	void translate(Vector coordinates);

	/*
	 * Anything added to an Orientation is rotated by the angle first.
	 */
	Orientation& operator += (real distance);
	Orientation& operator += (Vector v);

private:
	/*
	 * Calculate dx and dy based on angle.
	 */
	void calcUnits();

	Vector location;
	Vector dx, dy; // Unit vectors of angle
	Angle angle;
};

Orientation operator + (const Orientation& o, real distance);
Orientation operator + (const Orientation& o, const Vector& v);

#endif // _ORIENTATION_H_

