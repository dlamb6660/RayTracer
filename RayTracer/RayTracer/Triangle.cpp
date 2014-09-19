#include "StdAfx.h"
#include "Triangle.h"
#include "Plane.h"
#include <math.h>
#include <iostream>
using namespace std;

const double smallNum = 0.001; 

Triangle::Triangle(void)
	: v0(),v1(),v2(),n0(),n1(),n2()
{
	this->bbox = false;
	this->init = true;
}

Triangle::Triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2)
	: v0(p0),v1(p1),v2(p2)
{
	this->bbox = false;
	this->init = true;
	Vector cross = ((v2 - v0) ^ (v1 - v0));
	normal = cross / (sqrt(pow(cross.x,2) + pow(cross.y,2) + pow(cross.z,2))); // normalise the cross product
}

Triangle::Triangle(const Triangle& t)
	: v0(t.v0), v1(t.v1), v2(t.v2), n0(t.n0), n1(t.n1), n2(t.n2)
{
	this->bbox = false;
	this->init = true;
	Vector cross = ((v2 - v0) ^ (v1 - v0));
	normal = cross / (sqrt(pow(cross.x,2) + pow(cross.y,2) + pow(cross.z,2))); // normalise the cross product
}

Triangle::~Triangle(void)
{
}

// barycentric intersection
bool Triangle::intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const{
	//from Equation 19.10 in Ray Tracing From The Ground Up
	// [a b c][beta ]   [d]
	// [e f g][gamma] = [h]
	// [i j k][  t  ]   [l]
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

	double m = f*k - g*j, n = h*k - g*l, p = f*l - h*j;
	double q = g*i - e*k, s = e*j - f*i;

	double inv_denom = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if(beta < 0.0) return false;

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if(gamma < 0.0) return false;

	if(beta + gamma > 1.0) return false;

	double e3 = a * p - b * r + d * s;
	double t0 = e3 * inv_denom;

	if(t0 < smallNum) return false;

	t = t0;
	shadeRay.normal = normal;
	shadeRay.intersectionPoint = ray.origin + Vector(ray.direction.x * t, ray.direction.y * t, ray.direction.z * t);
	shadeRay.colour = baseColour;
	shadeRay.material = materialPtr;
	shadeRay.rayDirection = ray.direction;
	return true;
}

Point3D Triangle::getMinPoint() const{
	double min_x, min_y, min_z;

	if(v0.x < v1.x) min_x = v0.x;
	else min_x = v1.x;
	if(v2.x < min_x) min_x = v2.x;

	if(v0.y < v1.y) min_y = v0.y;
	else min_y = v1.y;
	if(v2.y < min_y) min_y = v2.y;

	if(v0.z < v1.z) min_z = v0.z;
	else min_z = v1.z;
	if(v2.z < min_z) min_z = v2.z;

	return Point3D(min_x, min_y, min_z);
}

Point3D Triangle::getMaxPoint() const{
	double max_x, max_y, max_z;

	if(v0.x > v1.x) max_x = v0.x;
	else max_x = v1.x;
	if(v2.x > max_x) max_x = v2.x;

	if(v0.y > v1.y) max_y = v0.y;
	else max_y = v1.y;
	if(v2.y > max_y) max_y = v2.y;

	if(v0.z > v1.z) max_z = v0.z;
	else max_z = v1.z;
	if(v2.z > max_z) max_z = v2.z;

	return Point3D(max_x, max_y, max_z);
}