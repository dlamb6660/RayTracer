#include "StdAfx.h"
#include "BoundingBox.h"
#include <iostream>
using namespace std;

const double smallNum = 0.001;

BoundingBox::BoundingBox(void)
{
	this->bbox = true;
	this->init = true;
}

BoundingBox::BoundingBox(const vector<GeometricShape*>& bC)
{
	this->boxContents = bC;
	this->bbox = true;
	this->init = true;
	//construct points of bounding box based on contents
	constructBox();
}

BoundingBox::BoundingBox(const BoundingBox& bb)
{
	this->boxContents = bb.boxContents;
	this->bbox = true;
	this->init = true;
	constructBox();
}

BoundingBox::~BoundingBox(void)
{
}

void BoundingBox::constructBox(void){
	for(int i=0; i<this->boxContents.size(); i++){
		Point3D min_p = this->boxContents[i]->getMinPoint();
		Point3D max_p = this->boxContents[i]->getMaxPoint();

		if(i==0){
			p0 = min_p;
			p1 = max_p;
		}
		else{
			if(min_p.x < p0.x) p0.x = min_p.x;
			if(min_p.y < p0.y) p0.y = min_p.y;
			if(min_p.z < p0.z) p0.z = min_p.z;

			if(max_p.x > p1.x) p1.x = max_p.x;
			if(max_p.y > p1.y) p1.y = max_p.y;
			if(max_p.z > p1.z) p1.z = max_p.z;
		}
	}
}

bool BoundingBox::intersect(const Ray& ray, double& t, ShadeRay& shadeRay) const{
	// Original code taken from book

	double ox = ray.origin.x, oy = ray.origin.y, oz = ray.origin.z;
	double dx = ray.direction.x, dy = ray.direction.y, dz = ray.direction.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if(a >= 0) {
		tx_min = (p0.x - ox) * a;
		tx_max = (p1.x - ox) * a;
	}
	else{
		tx_min = (p1.x - ox) * a;
		tx_max = (p0.x - ox) * a;
	}

	double b = 1.0 / dy;
	if(b >= 0) {
		ty_min = (p0.y - oy) * b;
		ty_max = (p1.y - oy) * b;
	}
	else{
		ty_min = (p1.y - oy) * b;
		ty_max = (p0.y - oy) * b;
	}

	double c = 1.0 / dz;
	if(c >= 0) {
		tz_min = (p0.z - oz) * c;
		tz_max = (p1.z - oz) * c;
	}
	else{
		tz_min = (p1.z - oz) * c;
		tz_max = (p0.z - oz) * c;
	}

	double t0,t1;

	// find largest entering t value

	if(tx_min > ty_min)	t0 = tx_min;
	else t0 = ty_min;
	if(tz_min > t0) t0 = tz_min;

	// find smallest exiting t value

	if(tx_max < ty_max)	t1 = tx_max;
	else t1 = ty_max;
	if(tz_max < t1) t1 = tz_max;

	if(t0 < t1 && t1 > smallNum) return true;
	else return false;
}

Point3D BoundingBox::getMinPoint() const{
	return p0;
}

Point3D BoundingBox::getMaxPoint() const{
	return p1;
}