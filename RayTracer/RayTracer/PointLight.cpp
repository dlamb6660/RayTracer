#include "StdAfx.h"
#include "PointLight.h"
#include <math.h>

PointLight::PointLight(void)
{
	position = Point3D(0);
	direction = Vector(0);
	intensity = 1;
	colour = RGB(255/255);
}

PointLight::PointLight(const Point3D& pos)
{
	position = pos;
	direction = Vector(0);
	intensity = 1;
	colour = RGB(255/255);
}

PointLight::PointLight(const Point3D& pos, const Vector& dir) // point light with optional direction
{
	position = pos;
	direction = dir;
	intensity = 3; // default intensity
	colour = RGB(255/255); // default white
}

PointLight::PointLight(const Point3D& pos, const Vector& dir, const float in)
{
	position = pos;
	direction = dir;
	intensity = in;
	colour = RGB(255/255); // default white
}

PointLight::PointLight(const Point3D& pos, const Vector& dir, const float in, const RGB col)
{
	position = pos;
	direction = dir;
	intensity = in;
	colour = RGB(col);
}

PointLight::PointLight(const Point3D& pos, const float in, const RGB col)
{
	position = pos;
	direction = Vector(0);
	intensity = in;
	colour = RGB(col);
}

PointLight::~PointLight(void)
{
}

Lights* PointLight::clone(void) const{
	return (new PointLight(*this));
}

Vector PointLight::getDirection(const Point3D& intersection) const{
	Vector temp = (position - intersection);
	return Vector(temp / (sqrt(pow(temp.x,2) + pow(temp.y,2) + pow(temp.z,2))));
}