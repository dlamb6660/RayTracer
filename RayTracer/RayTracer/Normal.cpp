#include "StdAfx.h"
#include "Normal.h"
#include <math.h>
#include <iostream>
using namespace std;

Normal::Normal(void)
	:x(0.0),y(0.0),z(0.0)
{
}

Normal::Normal(double a)
	:x(a),y(a),z(a)
{
}

Normal::Normal(double x0, double y0, double z0)
	:x(x0),y(y0),z(z0)
{
}

Normal::Normal(const Normal& n)
	:x(n.x),y(n.y),z(n.z)
{
}

Normal::Normal(const Vector& v)
	:x(v.x),y(v.y),z(v.z)
{
}

Normal::~Normal(void)
{
}

double Normal::operator* (const Vector& vec){ // dot product of normal . vector
	return ((x * vec.x) + (y * vec.y) + (z * vec.z));
}

Normal Normal::operator/ (const double scalar) const{
	return Normal(x / scalar, y / scalar, z / scalar);
}