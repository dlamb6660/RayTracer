class Materials;
#include "Point3D.h"
#include "Ray.h"
#include "ShadeRay.h"
#include "Normal.h"
#include "RGB.h"
#include <vector>
using namespace std;

#pragma once
class GeometricShape
{
public:
	Materials* materialPtr;
	RGB baseColour;
	bool init; // stores whether this shape has been initialised or not
	bool bbox; // stores whether this shape is a bounding box for other shapes or not
	vector<GeometricShape*> boxContents; // stores list of shapes that are inside bounding box. only used by bounding volumes

	virtual bool intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const = 0;
	virtual Point3D getMinPoint(void) const = 0;
	virtual Point3D getMaxPoint(void) const = 0;

	GeometricShape(void);
	GeometricShape(const GeometricShape& g);
	~GeometricShape(void);
};