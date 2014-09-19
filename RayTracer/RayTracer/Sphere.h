#include "GeometricShape.h"

#pragma once
class Sphere: public GeometricShape
{
public:
	Sphere(void);
	Sphere(const Point3D& cent, const double rad);
	Sphere(const Sphere& s);
	~Sphere(void);

	virtual bool intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const;
	virtual Point3D getMinPoint(void) const;
	virtual Point3D getMaxPoint(void) const;

	Point3D centre;
	double radius;
};