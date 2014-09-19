#include "StdAfx.h"
#include "View.h"


View::View(void)
	: pixelSize(1),imgWidth(16),imgHeight(16)
{
}

View::View(int res)
	: pixelSize(1),imgWidth(res),imgHeight(res),jitter_sample(1)
{
}
	
View::View(int imW, int imH)
	: pixelSize(1),imgWidth(imW),imgHeight(imH),jitter_sample(1)
{
}

View::View(int imW, int imH, int jit)
	: pixelSize(1),imgWidth(imW),imgHeight(imH),jitter_sample(jit)
{
}
	
View::View(int imW, int imH, int jit, float s)
	: pixelSize(s),imgWidth(imW),imgHeight(imH),jitter_sample(jit)
{
}

View::~View(void)
{
}
