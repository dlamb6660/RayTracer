#pragma once
class View
{
public:
	View(void);
	View(int res);
	View(int imW, int imH);
	View(int imW, int imH, int jit);
	View(int imW, int imH, int jit, float s);
	~View(void);

	int imgWidth, imgHeight; // in pixels
	int jitter_sample; // used for anti-aliasing. number of samples taken per pixel
	float pixelSize;
};