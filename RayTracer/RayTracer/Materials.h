#include "ShadeRay.h"
#include "RGB.h"
#include "Lights.h"
#include "AmbientLight.h"
#include "Scene.h"

#pragma once
class Materials
{
public:
	Materials(void);
	~Materials(void);

	float kd; // diffuse reflection coefficient
	float ka; // ambient reflection coefficient
	float ks; // specular reflection coefficient
	float kr; // reflectivity coefficient

	virtual RGB shadeSurface(ShadeRay& shadeRay, Lights& light, RGB& current) const = 0;
	virtual RGB getAmbientShading(ShadeRay& shadeRay, AmbientLight& ambient) const = 0;
	virtual RGB reflection(ShadeRay& shadeRay) const = 0;
};

