#include "StdAfx.h"
#include "Specular.h"
#include <math.h>
#include <iostream>
using namespace std;

const double inversePi = 0.3183098861837906715;

Specular::Specular(void)
{
	ka = 0.35;
	ks = 1;
	exp = 40;
	kd = 0.25;
	kr = 0.5;
}

Specular::~Specular(void)
{
}

RGB Specular::getAmbientShading(ShadeRay& shadeRay, AmbientLight& ambient)const{
	return RGB(shadeRay.colour * shadeRay.material->kd * (ambient.colour * ambient.intensity));
}

RGB Specular::shadeSurface(ShadeRay& shadeRay, Lights& light, RGB& current) const{
	double cosine = (shadeRay.normal * light.direction);
	if(cosine <= 0) { // if this is less than 0 then it means that the intersection point on the surface is not visible to the light source (usually due to object being convex)
		return RGB(0);
	}
	RGB lambert = RGB((light.colour * light.intensity) * cosine * shadeRay.material->kd);

	Vector lightDir = light.getDirection(shadeRay.intersectionPoint);
	double dot = lightDir * Vector(shadeRay.normal);
	double reflet = 2.0 * (lightDir * Vector(shadeRay.normal));

	Vector phongDir = lightDir - (Vector(shadeRay.normal) * reflet);
	Vector rayDir = shadeRay.rayDirection; // from eye to point
	double temp = phongDir * rayDir;
	double phongTerm = 0.0;

	RGB difference = RGB(0);
	if(temp > 0.0) {
		phongTerm = temp;
		phongTerm = pow(phongTerm, exp) * shadeRay.material->ks * light.intensity;
		RGB reflection = current + ((current + lambert) * phongTerm);
		difference = reflection - current;
	} 

	return lambert + difference;
}

RGB Specular::reflection(ShadeRay& shadeRay) const{
	RGB mirror = RGB(0);

	// full equation: ref_dir = shadeRay.rayDirection - ((shadeRay.normal * (shadeRay.normal . shadeRay.rayDirection)) * 2.0)
	double temp0 = shadeRay.normal * shadeRay.rayDirection;
	Vector temp1 = Vector(shadeRay.normal) * temp0;
	Vector temp2 = temp1 * 2.0;
	Vector ref_dir = shadeRay.rayDirection - temp2;
	Ray reflectedRay = Ray(shadeRay.intersectionPoint, ref_dir);
	
	shadeRay.depth += 1;

	mirror = shadeRay.scene.tracerPtr->raytrace(reflectedRay, shadeRay.depth) * shadeRay.material->kr;
	return mirror;
}