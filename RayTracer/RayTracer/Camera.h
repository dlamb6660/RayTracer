class Scene;
#include "Point3D.h"
#include "Vector.h"

#pragma once
class Camera
{
public:
	Point3D pinhole;
	Point3D target;
	Vector u,v,w, up;
	double focaldistance;

	Camera(void);
	Camera(const Point3D& p, const Point3D& t, const double f);
	Camera(const Camera& camera);
	~Camera(void);
};

