#include "Vector.h"
//#include "Matrix.h"

#pragma once
class Point3D
{
public:
	double x,y,z;

	Point3D(void);
	Point3D(const double a);
	Point3D(const double x0, const double y0, const double z0);
	Point3D(const Point3D& p);
	~Point3D(void);

	Vector operator- (const Point3D sub) const;
	Point3D operator+ (const Vector add) const;
};

