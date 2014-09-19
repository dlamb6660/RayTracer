#include "StdAfx.h"
#include "Lambert.h"
#include <vector>
#include <iostream>
using namespace std;
const double inversePi = 0.3183098861837906715;

Lambert::Lambert(void)
{
	ka = 0.2;
	kd = 0.5;
}

Lambert::~Lambert(void)
{
}

RGB Lambert::getAmbientShading(ShadeRay& shadeRay, AmbientLight& ambient) const{
	return RGB(shadeRay.colour * shadeRay.material->kd * (ambient.colour * ambient.intensity));
}

RGB Lambert::shadeSurface(ShadeRay& shadeRay, Lights& light, RGB& current) const{
	// this is where we do each iteration of this part of the equation...:
	// sumj[(kd*cd / pi) * (ls,j*cl,j) * (n.lj)]

	float cosine = (shadeRay.normal * light.direction);
	if(cosine <= 0) { // if this is less than 0 then it means that the intersection point on the surface is not visible to the light source (usually due to object being convex)
		return RGB(0);
	}
	return (light.colour * light.intensity) * cosine * shadeRay.material->kd;
}

RGB Lambert::reflection(ShadeRay& shadeRay) const{
	return RGB(0); // lambert materials are not perfect specular
}