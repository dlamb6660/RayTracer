#include "GeometricShape.h"
#include "Tracer.h"
#include "View.h"
#include "RGB.h"
#include "Ray.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "BoundingBox.h"
#include "PointLight.h"
#include "Lights.h"
#include "AmbientLight.h"
#include <list>
#include <vector>
using namespace std;

#pragma once
class Scene
{
public:
	vector<GeometricShape*> objectList;
	vector<Lights*> lightList;

	Tracer* tracerPtr;
	AmbientLight* ambientLight;
	vector<GeometricShape*> shadowCache;
	int n; // stores size of shadow cache
	int cache_ctr; // stores number of objects in cache at the moment

	Scene(void);
	~Scene(void);

	void addShape(GeometricShape* shapePtr);
	void constructObjects(void);
	void addLight(Lights* lightPtr);
	void constructLights(void);
	bool intersectLevel(const Ray& ray, double& t, ShadeRay& shadeRay, const vector<GeometricShape*>& level); // intersect with current level of objects in bounding hierarchy
	bool shadowIntersection(const Ray& ray, double& t, double& distance);
	GeometricShape* shadowIntersectLevel(const Ray& ray, double& t, const vector<GeometricShape*>& level);
	RGB getLighting(ShadeRay& shadeRay, Point3D& hitLocation);
};