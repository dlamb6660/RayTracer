#include "StdAfx.h"
#include "Tracer.h"
#include <list>
#include <iostream>
#include "Scene.h"
#include "RGB.h"

using namespace std;

const int DEPTH_LIMIT = 5;

Tracer::Tracer(void)
//	: scenePtr(NULL)
{
}

Tracer::Tracer(Scene* sp)
	: scenePtr(sp)
{
}

Tracer::~Tracer(void)
{
//	if(scenePtr) scenePtr = NULL;
}

RGB Tracer::raytrace(Ray& ray, int depth) const{
	if(depth > DEPTH_LIMIT) return RGB(0);
	else{
		double t;
		RGB col;
		ShadeRay shadeRay(*scenePtr);
		shadeRay.depth = depth;
		if(scenePtr->intersectLevel(ray,t,shadeRay,scenePtr->objectList)){
			//cout << "hit an object" << endl;
			Point3D hitLocation = ray.origin + (ray.direction * t);
			// we hit an object, so find lighting at the hit location
			col = scenePtr->getLighting(shadeRay, hitLocation);
			return col;
		}
		else{
			return shadeRay.colour; // <- background colour
		}
	}
}