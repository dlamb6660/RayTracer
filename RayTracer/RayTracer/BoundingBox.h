#include "GeometricShape.h"
#include <vector>
using namespace std;

#pragma once
class BoundingBox: public GeometricShape
{
public:
	BoundingBox(void);
	BoundingBox(const vector<GeometricShape*>& bC);
	BoundingBox(const BoundingBox& bb);
	~BoundingBox(void);

	virtual bool intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const;
	void constructBox(void);

	virtual Point3D getMinPoint(void) const;
	virtual Point3D getMaxPoint(void) const;

	Point3D p0; // one corner of the box
	Point3D p1; // opposite corner of box
};

