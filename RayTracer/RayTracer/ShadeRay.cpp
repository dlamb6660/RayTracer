#include "StdAfx.h"
#include "ShadeRay.h"


//ShadeRay::ShadeRay(void)
//	: normal(),intersectionPoint()
//{
//}

ShadeRay::ShadeRay(Scene& sp)
	: intersectionPoint(),normal(),scene(sp)
{
}


ShadeRay::~ShadeRay(void)
{
}
