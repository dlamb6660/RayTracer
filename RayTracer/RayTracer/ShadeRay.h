class Scene;
class Materials;
#include "Normal.h"
#include "Point3D.h"
#include "RGB.h"
#include "Ray.h"

#pragma once
class ShadeRay
{
public:
	ShadeRay(Scene& sp);
	~ShadeRay(void);

	Normal normal;
	Point3D intersectionPoint;
	RGB colour;
	Materials* material;
	Vector rayDirection;
	int depth;
	Scene& scene;
};