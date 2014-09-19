#include "StdAfx.h"
#include "Sphere.h"
#include <math.h>
#include <iostream>
using namespace std;

const double smallNum = 0.001; // used to compare locations of intersections in relation to an object.
							   // instead using 0 (which is correct) as this causes errors for assigning materials

Sphere::Sphere(void)	
	:centre(0.0),radius(1.0)
{
	this->bbox = false;
	this->init = true;
}

Sphere::Sphere(const Point3D& cent, double rad)
	: 	GeometricShape(),centre(cent),radius(rad)
{
	this->bbox = false;
	this->init = true;
}

Sphere::Sphere (const Sphere& s)
	: 	GeometricShape(s),centre(s.centre),radius(s.radius)
{
	this->bbox = false;
	this->init = true;
}

bool Sphere::intersect(const Ray& ray, double&t, ShadeRay& shadeRay) const{
	//cout << "intersect sphere" << endl;
	// Equation 3.9 from book:
	// a = ray.direction . ray.direction
	// b = 2 * (ray.origin - sphere.centre) . ray.direction
	// c = (ray.origin - sphere.centre) * (ray.origin - sphere.centre) - sphere.radius^2
	Vector v = (ray.origin - centre);
	double a = ray.direction * ray.direction;
	double b = (v * 2.0) * ray.direction;
	double c = ((v.x * v.x) + (v.y * v.y) + (v.z * v.z)) - (radius * radius);

	// d = b^2 - 4*a*c		<-discriminant
	// if(d < 0) then no intersection
	double d = (b*b) - 4*a*c;
	if(d < 0) return false;
	else{
		// quadratic roots:
		double root0 = (-b - sqrt(d)) / (2 * a); // smaller root
		double root1 = (-b + sqrt(d)) / (2 * a); // larger root

		// smallest positive root is intersection point
		if(root0 > smallNum){
			t = root0;
			Vector tvec = Vector(ray.direction.x * root0, ray.direction.y * root0, ray.direction.z * root0);
			Normal n = (v + tvec) / radius;
			shadeRay.normal = n / (sqrt(pow(n.x,2) + pow(n.y,2) + pow(n.z,2))); // normalise
			shadeRay.intersectionPoint = ray.origin + tvec;
			shadeRay.material = materialPtr;
			shadeRay.colour = baseColour;
			shadeRay.rayDirection = ray.direction;
			return true;
		}
		// else, if larger root is real, return this
		else if(root1 > smallNum){
			t = root1;
			Vector tvec = Vector(ray.direction.x * root1, ray.direction.y * root1, ray.direction.z * root1);
			Normal n = (v + tvec) / radius;
			shadeRay.normal = n / (sqrt(pow(n.x,2) + pow(n.y,2) + pow(n.z,2))); // normalise
			shadeRay.intersectionPoint = ray.origin + tvec;
			shadeRay.material = materialPtr;
			shadeRay.colour = baseColour;
			shadeRay.rayDirection = ray.direction;
			return true;
		}
		else{
			// neither root was positive, so no intersection found
			return false;
		}
	}

	return false;
}

Point3D Sphere::getMinPoint() const{
	Point3D min = Point3D(-radius);
	return (centre + min);
}

Point3D Sphere::getMaxPoint() const{
	Point3D max = Point3D(radius);
	return (centre + max);
}