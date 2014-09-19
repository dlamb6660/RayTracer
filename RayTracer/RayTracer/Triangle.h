#include "GeometricShape.h"

#pragma once
class Triangle: public GeometricShape
{
public:
	Triangle(void);
	Triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2); 
	Triangle(const Triangle& t);
	~Triangle(void);

	virtual bool intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const;
	bool HitInsideTriangle(const Point3D p, const Point3D a, const Point3D b, const Point3D c) const; 
	bool BarycentricIntersection(const Point3D p) const; 
	bool SameSide(const Point3D p0, const Point3D p1, const Point3D a, const Point3D b) const;
	virtual Point3D getMinPoint(void) const;
	virtual Point3D getMaxPoint(void) const;

	Point3D v0,v1,v2;
	Normal normal,n0,n1,n2;
};