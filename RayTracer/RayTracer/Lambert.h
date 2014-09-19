#include "Materials.h"

#pragma once
class Lambert: public Materials
{
public:
	Lambert(void);
	~Lambert(void);

	virtual RGB shadeSurface(ShadeRay& shadeRay, Lights& light, RGB& current) const;
	virtual RGB getAmbientShading(ShadeRay& shadeRay, AmbientLight& ambient) const;
	virtual RGB reflection(ShadeRay& shadeRay) const;
};

