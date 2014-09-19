#include "StdAfx.h"
#include "RGB.h"
#include <iostream>
using namespace std;

RGB::RGB(void)
	:r(0.5),g(0.5),b(0.5)
	//:r(127.5),g(127.5),b(127.5)
{
}
RGB::RGB(float a)
	:r(a),g(a),b(a)
{
	//if(a>1)cout << "**************Hey, a > 1 you prick " << a << endl;
	// must be between 0 and 1 for * and + operations to work properly
	/*if(a > 1){
		r /= 255;
		g /= 255;
		b /= 255;
	}*/
}
RGB::RGB(float r0,float g0,float b0)
	:r(r0),b(b0),g(g0)
{
	//if(r0>1)cout << "*************Hey, r0 > 1 you prick " << r0 << endl;
	//if(g0>1)cout << "*************Hey, g0 > 1 you prick " << g0 << endl;
	//if(b0>1)cout << "*************Hey, b0 > 1 you prick " << b0 << endl;
	// must be between 0 and 1 for * and + operations to work properly
	/*if(r0 > 1 || g0 > 1 || b0 > 1){
		r /= 255;
		g /= 255;
		b /= 255;
	}*/
}

RGB RGB::operator* (const double scalar) {
	return RGB(r * scalar, g * scalar, b * scalar);
}

RGB RGB::operator* (const RGB col) {
	return RGB(r * col.r, g * col.g, b * col.b);
}

RGB RGB::operator+ (const RGB col) {
	return RGB(r + col.r, g + col.g, b + col.b);
}

RGB RGB::operator- (const RGB col) {
	return RGB(r - col.r, g - col.g, b - col.b);
}

RGB RGB::operator/ (const double scale) {
	return RGB(r / scale, g / scale, b / scale);
}

RGB::~RGB(void)
{
}

double RGB::CalculateDifference(const RGB& col){
	// scale values up to 0->255 so that values are easier to analyse
	return (sqrt(pow((r*255.0)-(col.r*255.0),2) + pow((g*255.0)-(col.g*255.0),2) + pow((b*255.0)-(col.b*255.0),2)));
}