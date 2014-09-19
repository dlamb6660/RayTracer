#include "Lights.h"
#pragma once
class AmbientLight: public Lights
{
public:
	AmbientLight(void);
	AmbientLight(const float in);
	AmbientLight(const float in, const RGB col);
	~AmbientLight(void);

	virtual Lights* clone(void) const;
	virtual Vector getDirection(const Point3D& intersection) const;
};

