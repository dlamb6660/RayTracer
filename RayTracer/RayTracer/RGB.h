#pragma once
class RGB
{
public:
	float r;
	float g;
	float b;

	RGB(void);
	RGB(float a);
	RGB(float r0,float g0,float b0);
	~RGB(void);

	RGB operator* (const double scalar);
	RGB operator* (const RGB col);
	RGB operator+ (const RGB col);
	RGB operator- (const RGB col);
	RGB operator/ (const double scale);

	double CalculateDifference(const RGB& col);
};

