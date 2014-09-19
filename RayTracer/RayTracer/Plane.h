#include "GeometricShape.h"

#pragma once
class Plane: public GeometricShape
{
public:
	Plane(void);
	Plane(const Point3D& p, const Normal& n);
	Plane(const Plane& p);
	~Plane(void);

	virtual bool intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const;
	virtual Point3D getMinPoint(void) const;
	virtual Point3D getMaxPoint(void) const;
	
	Point3D point;
	Normal normal;
};

