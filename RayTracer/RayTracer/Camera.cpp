#include "StdAfx.h"
#include "Camera.h"
#include <math.h>
#include <iostream>
using namespace std;
	// from slides
	// w = pinhole-target / |pinhole-target|
	// u = up x w / |up x w|
	// v = w x u

Camera::Camera(void)
	: pinhole(0,0,0),target(0,0,100),up(0,1,0),focaldistance(50)
{
	Vector pt = pinhole - target;
	w = pt / (sqrt(pow(pt.x,2) + pow(pt.y,2) + pow(pt.z,2)));
	Vector uw = up ^ w;
	u = uw / (sqrt(pow(uw.x,2) + pow(uw.y,2) + pow(uw.z,2)));
	v = w ^ u;
}

Camera::Camera(const Point3D& p, const Point3D& t, const double f)
	: pinhole(p),target(t),focaldistance(f),up(0,1,0)
{
	Vector pt = pinhole - target;
	w = pt / (sqrt(pow(pt.x,2) + pow(pt.y,2) + pow(pt.z,2)));
	Vector uw = up ^ w;
	u = uw / (sqrt(pow(uw.x,2) + pow(uw.y,2) + pow(uw.z,2)));
	v = w ^ u;
}


Camera::Camera(const Camera& cam)
	: pinhole(cam.pinhole),target(cam.target),up(cam.up),u(cam.u),v(cam.v),w(cam.w),focaldistance(cam.focaldistance)
{
}

Camera::~Camera(void)
{
}
