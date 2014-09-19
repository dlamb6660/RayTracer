#include "Materials.h"

#pragma once
class Specular: public Materials
{
public:
	Specular(void);
	~Specular(void);

	virtual RGB shadeSurface(ShadeRay& shadeRay, Lights& light, RGB& current) const;
	virtual RGB getAmbientShading(ShadeRay& shadeRay, AmbientLight& ambient) const;
	virtual RGB reflection(ShadeRay& shadeRay) const;

	double exp;
};

