#include "Vector.h"
#include "Point3D.h"


#pragma once
class Ray
{
public:
	Ray(void);
	Ray(const Point3D& orig, const Vector& dir);
	Ray(const Ray& ray);
	~Ray(void);

	Point3D origin;
	Vector direction;
	//Vector inv_dir;
	//int sign[3];
};