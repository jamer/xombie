#include <math.h>

#include "vector.h"

Vector::Vector()
	: x(0), y(0)
{
}

Vector::Vector(real _x, real _y)
	: x(_x), y(_y)
{
}

real Vector::distanceTo(const Vector& v)
{
	int b = x - v.x;
	int c = y - v.y;
	return (real)sqrt(b*b + c*c);
}

Vector& Vector::operator += (const Vector& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector& Vector::operator *= (const Vector& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

Vector& Vector::operator += (real distance)
{
	x += distance;
	y += distance;
	return *this;
}

Vector& Vector::operator *= (real distance)
{
	x *= distance;
	y *= distance;
	return *this;
}

Vector operator + (const Vector& vec, real distance)
{
	Vector v = vec;
	v += distance;
	return v;
}

Vector operator + (const Vector& lhs, const Vector& rhs)
{
	Vector v = lhs;
	v += rhs;
	return v;
}

Vector operator * (const Vector& vec, real distance)
{
	Vector v = vec;
	v *= distance;
	return v;
}

Vector operator * (const Vector& lhs, const Vector& rhs)
{
	Vector v = lhs;
	v *= rhs;
	return v;
}

