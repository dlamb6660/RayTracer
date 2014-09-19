#include "Lights.h"
#pragma once
class PointLight: public Lights
{
public:
	PointLight(void);
	PointLight(const Point3D& pos);
	PointLight(const Point3D& pos, const Vector& dir);
	PointLight(const Point3D& pos, const Vector& dir, const float in);
	PointLight(const Point3D& pos, const Vector& dir, const float in, const RGB col);
	PointLight(const Point3D& pos, const float in, const RGB col);
	~PointLight(void);

	virtual Lights* clone(void) const;
	virtual Vector getDirection(const Point3D& intersection) const;
};

