#include "Vector.h"
#include "Point3D.h"
//#include "Matrix.h"

#pragma once
class Normal
{
public:
	double x,y,z;

	Normal(void);
	Normal(double a);
	Normal(double x0, double y0, double z0);
	Normal(const Normal& n);
	Normal(const Vector& v);
	~Normal(void);

	double operator* (const Vector& vec); // dot product of normal . vector
	Normal operator/ (const double scalar) const;
};

double operator* (const Vector& vec, const Normal& norm); // dot product of vector . normal
inline double operator* (const Vector& vec, const Normal& norm) {
	return (vec.x * norm.x + vec.y * norm.y + vec.z * norm.z);     
}