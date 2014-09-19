#include "StdAfx.h"
#include "Vector.h"
#include "Normal.h"
#include "Point3D.h"
#include <iostream>
using namespace std;

Vector::Vector(void)
	 : x(0.0), y(0.0), z(0.0)							
{
}

Vector::Vector(double a)
	 : x(a), y(a), z(a)							
{
}

Vector::Vector(double x0, double y0, double z0)	 
	: x(x0), y(y0), z(z0)
{
}

Vector::Vector(const Vector& vector)
	: x(vector.x), y(vector.y), z(vector.z)
{
}

Vector::Vector(const Normal& n)	 
	: x(n.x), y(n.y), z(n.z)
{
}

Vector::Vector(const Point3D& p)	 
	: x(p.x), y(p.y), z(p.z)
{
}

Vector::~Vector(void)
{
}

double Vector::operator* (const Vector& vec){ // dot product
	return ((x * vec.x) + (y * vec.y) + (z * vec.z));
}

Vector Vector::operator^ (const Vector& vec){ // cross product
	return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

Vector Vector::operator* (const double scalar){
	return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator+ (const Vector& vec){
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

Vector Vector::operator+ (const double scalar){
	return Vector(x + scalar, y + scalar, z + scalar);
}

Vector Vector::operator/ (const double scalar){
	return Vector(x / scalar, y / scalar, z / scalar);
}

Vector Vector::operator- (const Vector& vec){
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

Vector Vector::operator- (const double scalar){
	return Vector(x - scalar, y - scalar, z - scalar);
}