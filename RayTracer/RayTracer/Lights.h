#include "Point3D.h"
#include "RGB.h"

#pragma once
class Lights
{
public:
	Lights(void);
	~Lights(void);

	virtual Lights* clone(void) const = 0;
	virtual Vector getDirection(const Point3D& intersection) const = 0;

	Point3D position;
	Vector direction;
	float intensity;
	RGB colour;
};

