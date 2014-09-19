#include "StdAfx.h"
#include "Plane.h"
#include <iostream>
using namespace std;

const double smallNum = 0.001;

Plane::Plane(void)	
	: point(0.0),normal(0.0, 1.0, 0.0)
{
	baseColour = NULL;
	this->bbox = false;
	this->init = true;
}

Plane::Plane(const Point3D& p, const Normal& n)
	:	point(p),normal(n)
{
	baseColour = NULL;
	this->bbox = false;
	this->init = true;
}

Plane::Plane(const Plane& p) 
	:	GeometricShape(p),point(p.point),normal(p.normal)
{
	baseColour = NULL;
	this->bbox = false;
	this->init = true;
}

bool Plane::intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const {
	// Equation 3.4 - t = (a - o) n / (d . n) ...where:
	// t is distance along ray that intersection occurs
	// a is point on plane ; o is origin of ray ; (a - o) is vector from origin to point on plane
	// n is normal to the plane that is intersected
	// d is direction vector of the ray

	float t0 = ((point - ray.origin) * Vector(normal)) / (ray.direction * Vector(normal));
	// if (virtually) negative
	if(t0 < smallNum){
		return false;
	}
	else{ // else we have an intersection, so return info about intersection
		t = t0;
		shadeRay.normal = normal;
		shadeRay.intersectionPoint = ray.origin + Vector(ray.direction.x * t, ray.direction.y * t, ray.direction.z * t);
		shadeRay.colour = baseColour;
		shadeRay.material = materialPtr;
		shadeRay.rayDirection = ray.direction;
		return true;
	}
}

Point3D Plane::getMinPoint() const{
	//shouldnt ever be used as planes can't go inside bounding boxes
	cout << "Whoops! ... in BoundingBox::getMinPoint()" << endl;
	return Point3D(0);
}

Point3D Plane::getMaxPoint() const{
	//shouldnt ever be used
	cout << "Whoops! ... in BoundingBox::getMaxPoint()" << endl;
	return Point3D(0);
}