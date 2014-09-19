class Point3D;
class Normal;
//#include "Matrix.h"

#pragma once
class Vector
{
public:
	Vector(void);
	Vector(double a);
	Vector(double x0, double y0, double z0);
	Vector(const Vector& v);
	Vector(const Normal& n);
	Vector(const Point3D& p);
	~Vector(void);

	double operator* (const Vector& vec); // dot product
	Vector operator^ (const Vector& vec); // cross product
	Vector operator* (const double scalar);
	//Vector operator* (const double scalar) const;
	Vector operator+ (const Vector& vec);
	Vector operator+ (const double scalar);
	Vector operator/ (const Vector& vec);
	Vector operator/ (const double scalar);
	Vector operator- (const Vector& vec);
	Vector operator- (const double scalar);

	double x, y, z;
};

