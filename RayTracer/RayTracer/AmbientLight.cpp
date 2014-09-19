#include "StdAfx.h"
#include "AmbientLight.h"


AmbientLight::AmbientLight(void)
{
	intensity = 0.75;
	position = Point3D(0);
	direction = Vector(0); // no direction
	colour = RGB(255/255); // default white
}

AmbientLight::AmbientLight(const float in)
{
	intensity = in;
	position = Point3D(0);
	direction = Vector(0);
	colour = RGB(255/255);
}

AmbientLight::AmbientLight(const float in, const RGB col)
{
	intensity = in;
	position = Point3D(0);
	direction = Vector(0);
	colour = RGB(col);
}

AmbientLight::~AmbientLight(void)
{
}

Lights* AmbientLight::clone(void) const{
	return (new AmbientLight(*this));
}

Vector AmbientLight::getDirection(const Point3D& intersection) const{
	return direction;
}