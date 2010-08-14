#ifndef _VECTOR_H_
#define _VECTOR_H_

/*
 * An X-Y location.
 */

typedef double real;
typedef double Angle;

class Vector
{
public:
	Vector();
	Vector(real x, real y);

	Vector& operator += (const Vector& v);
	Vector& operator *= (const Vector& v);

	Vector& operator += (real distance);
	Vector& operator *= (real distance);

	real x, y;
};

Vector operator + (const Vector& v, real distance);
Vector operator + (const Vector& lhs, const Vector& rhs);

Vector operator * (const Vector& v, real distance);
Vector operator * (const Vector& lhs, const Vector& rhs);

#endif

