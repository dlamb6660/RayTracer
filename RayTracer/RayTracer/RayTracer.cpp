// RayTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "RGB.h"
#include "Scene.h"
#include "View.h"
#include "Tracer.h"
using namespace std;

void main(void)
{
	const int imgWidth = 1000, imgHeight = 600;
	const int high_jitter = 1,low_jitter = 1;
	const int thresh0 = 50, thresh1 = 80;

	clock_t startTime = clock(); // Start timer
	double secondsPassed = 0, minutesPassed, milliseconds;
	Ray ray;
	ray.direction = Vector(0,0,1);
	int jitter = low_jitter;// samples taken is jitter^2
	RGB pixel;
	RGB prev_pixel; // used for adaptive sampling

	FILE *outfile = fopen("output.ppm","wb");
	(void) fprintf(outfile,"P6\n%d %d\n255\n", imgWidth, imgHeight);

	// Initialise camera model, lights model, scene to be rendered, viewplane to project through
	Scene scene = Scene();
	View viewplane = View(imgWidth, imgHeight);
	Camera camera = Camera(Point3D(0,-100,-400), Point3D(0,0,100), 500.0);
	ray.origin = camera.pinhole;

	// loop through pixels
	for(int _y=0; _y<viewplane.imgHeight; _y++){
		for(int _x=0; _x<viewplane.imgWidth; _x++){
			pixel = RGB(0);
			// sample across each pixel
			for(int _v = 0; _v < jitter; _v++){ // up/down pixel
				for(int _u = 0; _u < jitter; _u++){ // left/right pixel
					Vector direction = ( ( (camera.u * (viewplane.pixelSize * (_x - viewplane.imgWidth/2    + (_u + (float)rand()/(float)RAND_MAX) / jitter   ))) + 
						(camera.v * (viewplane.pixelSize * (_y - viewplane.imgHeight/2    + (_v + (float)rand()/(float)RAND_MAX) / jitter   ))) ) - 
						(camera.w*camera.focaldistance) );

					Vector normalised = direction / (sqrt(pow(direction.x,2) + pow(direction.y,2) + pow(direction.z,2)));
					ray.direction = normalised;

					RGB pixel_0 = scene.tracerPtr->raytrace(ray, 0); // <- where 0 is initial ray depth
					
					//gamut fix
					if(pixel_0.r < 0.0)pixel_0.r = 0;
					if(pixel_0.r > 1.0)pixel_0.r = 1;
					if(pixel_0.g < 0.0)pixel_0.g = 0;
					if(pixel_0.g > 1.0)pixel_0.g = 1;
					if(pixel_0.b < 0.0)pixel_0.b = 0;
					if(pixel_0.b > 1.0)pixel_0.b = 1;

					pixel = pixel + pixel_0;
				}
			}
			// take average of sampling value, where number of samples taken is jitter squared
			pixel = pixel / pow(jitter,2.0);

			// adaptive sampling - change sampling rate based on difference between last two pixels
			double difference = pixel.CalculateDifference(prev_pixel);
			// if small difference and using high rate, change to low rate
			if(difference <= thresh0 && jitter == high_jitter && jitter != low_jitter){
				jitter = low_jitter;
			}
			// if big difference and using low rate, change to high rate and re-sample that pixel (at high rate)
			if(difference > thresh1 && jitter == low_jitter && jitter != high_jitter){
				jitter = high_jitter;
				// re-do pixel in high sampling rate
				if(_x == 0){
					_x = viewplane.imgWidth-1;
					_y--; // we know that either _x or _y have definitely incremented, so don't need special case for _y==0 as well
				}
				else{
					_x--;
				}
			}
			// if we are not re-sampling, write pixel to ppm file
			else{
				static unsigned char col[3];
				col[0] = (pixel.r * 255.0);
				col[1] = (pixel.g * 255.0);
				col[2] = (pixel.b * 255.0);
			
				(void) fwrite(col, 1, 3, outfile);

				prev_pixel = pixel;
			}
		}
		cout << (_y * 100 / viewplane.imgHeight) << "%\r";
	}

	(void) fclose(outfile);
	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	if(secondsPassed >= 60){
		minutesPassed = floor(secondsPassed / 60);
		secondsPassed = (int)secondsPassed % 60;
		cout << "Render completed in: " << minutesPassed << "m" << secondsPassed << "s" << endl;
	}
	else{
		cout << "Render completed in: " << secondsPassed << "s" << endl;
	}
	system("pause");
}