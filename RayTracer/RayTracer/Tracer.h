class Scene;
#include "RGB.h"
#include "Ray.h"

#pragma once
class Tracer
{
public:
	Tracer(void);
	Tracer(Scene* sp);
	~Tracer(void);

	RGB raytrace(Ray& ray, int depth) const;

	Scene* scenePtr;
};

