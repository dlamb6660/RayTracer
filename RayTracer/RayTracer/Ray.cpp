#include "StdAfx.h"
#include "Ray.h"


Ray::Ray(void)
	: origin(0.0), direction(0.0,0.0,1.0)
{
}

Ray::Ray(const Point3D& orig, const Vector& dir)
	: origin(orig), direction(dir)
{
}

Ray::Ray(const Ray& r)
	: origin(r.origin), direction(r.direction)
{
}

Ray::~Ray(void)
{
}