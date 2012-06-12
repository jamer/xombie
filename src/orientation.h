#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_

/*
 * A position and direction.
 */

#include "rotation.h"
#include "point.h"

class Orientation
{
public:
	Orientation();
	Orientation(const Point location, const angle theta);

	angle getAngle() const;
	Point getLocation() const;

	void setAngle(const angle theta);
	void setLocation(const Point location);
	void setLocation(const real x, const real y);

	/*
	 * Not rotated.
	 */
	void translate(const Point coords);
	void translate(const real x, const real y);

	/*
	 * Anything added to an Orientation is transformed by the rotation first.
	 */
	Orientation& operator += (const real distance);
	Orientation& operator += (const Point coords);

private:
	Point location;
	Rotation rotation;
};

Orientation operator + (const Orientation& o, real distance);
Orientation operator + (const Orientation& o, const Point& v);

#endif

