#include "StdAfx.h"
#include "Point3D.h"


Point3D::Point3D(void)
	: x(0.0), y(0.0), z(0.0)
{
}

Point3D::Point3D(const double a)
	: x(a), y(a), z(a)
{
}
Point3D::Point3D(const double x0, const double y0, const double z0)
	: x(x0), y(y0), z(z0)
{
}
Point3D::Point3D(const Point3D& p)
	: x(p.x), y(p.y), z(p.z)
{
}

Point3D::~Point3D(void)
{
}


Vector Point3D::operator- (const Point3D sub) const{
	return Vector(x - sub.x, y - sub.y, z - sub.z);
}

Point3D Point3D::operator+ (const Vector add) const{
	return Point3D(x + add.x, y + add.y, z + add.z);
}