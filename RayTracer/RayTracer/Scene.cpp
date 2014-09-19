#include "StdAfx.h"
#include "Scene.h"
#include "Lambert.h"
#include "Specular.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

Scene::Scene(void)
	: tracerPtr(NULL)
{
	tracerPtr = new Tracer(this);
	constructObjects();
	constructLights();
	Plane* p = new Plane();
	p->init = false;
	n = 5;//cache size
	cache_ctr = 0;//current number of shapes in cache
	for(int i=0; i<n; i++){
		shadowCache.push_back(p);
	}
}

Scene::~Scene(void)
{
}

void Scene::addShape(GeometricShape* shapePtr) {  
	objectList.push_back(shapePtr);
}

bool Scene::intersectLevel(const Ray& ray, double& t, ShadeRay& shadeRay, const vector<GeometricShape*>& level){
	ShadeRay shadeRay0(*this);
	shadeRay.colour = RGB(40/255);// <- backgroundcolour
	bool result = false;
	double t0;
	t = 10000000000000;

	// check if ray hits any objects in scene
	for(int i=0; i<level.size(); i++){
		// if it does hit an objects, return colour of object
		if(level[i]->intersect(ray, t0, shadeRay0)){
			// if the current object is a bounding box, then intersect the objects inside box
			if(level[i]->bbox){
				// this will return closest intersection inside level (t0) and relevant info (shadeRay0), so from here we can carry on as normal
				if(intersectLevel(ray, t0, shadeRay0, level[i]->boxContents)){
					// if closest intersection so far
					if(t0 < t){
						t = t0;
						shadeRay.colour = shadeRay0.colour;
						shadeRay.intersectionPoint = shadeRay0.intersectionPoint;
						shadeRay.material = shadeRay0.material;
						shadeRay.normal = shadeRay0.normal;
						shadeRay.rayDirection = shadeRay0.rayDirection;
						result = true;
					}
				}
			}
			else{
				// if closest intersection so far
				if(t0 < t){
					t = t0;
					shadeRay.colour = shadeRay0.colour;
					shadeRay.intersectionPoint = shadeRay0.intersectionPoint;
					shadeRay.material = shadeRay0.material;
					shadeRay.normal = shadeRay0.normal;
					shadeRay.rayDirection = shadeRay0.rayDirection;
					result = true;
				}
			}
		}
		else{
			// miss
		}
	}
	return result;
}

bool Scene::shadowIntersection(const Ray& ray, double& t, double& distance){
	double t0;
	ShadeRay dontcare(*this);
	GeometricShape* shape;

	// check objects stored in shadow cache first
	for(int i=0; i<n; i++){
		if(shadowCache[i]->init){
			if(shadowCache[i]->intersect(ray, t0, dontcare)){
				if(t0 < distance){ // if intersected with an object BEFORE the light source
					if(i>1){
						//rearrange shadow cache, so that intersected object is first in list
						GeometricShape* g = shadowCache[i];
						int index = min(n-1,cache_ctr);
						shadowCache[index] = shadowCache[0];
						shadowCache[0] = g;
						// rather than rearranging entire order, which is expensive, I just put the latest object to the front of the list, and move the previous head of the list to the back
					}
					return true;
				}
			}
		}
		else{
			break; // there are only i-1 objects stored in shadow cache, and we haven't hit any of them, so break
		}
	}

	// check if ray hits any objects in scene
	for(int i=0; i<objectList.size(); i++){
		// if object is a bounding volume, intersect contents of bounding volume
		if(objectList[i]->bbox){
			shape = shadowIntersectLevel(ray, t0, objectList[i]->boxContents);
			// if closest intersection so far
			if(shape != NULL && t0 < distance){
				// insert object into shadow cache
				int index = min(n-1,cache_ctr);
				shadowCache[index] = shadowCache[0];
				shadowCache[0] = shape;
				cache_ctr += 1;
				return true;
			}
		}
		else{
			// if it does hit an objects before the light, then return true
			if(objectList[i]->intersect(ray, t0, dontcare)){
				if(t0 < distance){ // if intersected with an object BEFORE the light source
					// insert object into shadow cache
					int index = min(n-1,cache_ctr);
					shadowCache[index] = shadowCache[0];
					shadowCache[0] = objectList[i];
					cache_ctr += 1;
					// again, I dont bother rearranging the whole list here to save computation
					return true;
				}
			}
		}
	}
	return false;
}

GeometricShape* Scene::shadowIntersectLevel(const Ray& ray, double& t, const vector<GeometricShape*>& level){
	ShadeRay dontcare(*this);
	double t0;
	t = 10000000000000;

	// check if ray hits any objects in scene
	for(int i=0; i<level.size(); i++){
		// if it does hit an objects, return colour of object
		if(level[i]->intersect(ray, t0, dontcare)){
			// if the current object is a bounding box, then intersect the objects inside box
			if(level[i]->bbox){
				// this will return closest intersection inside level (t0) and relevant info (shadeRay0), so from here we can carry on as normal
				GeometricShape* s = shadowIntersectLevel(ray, t0, level[i]->boxContents);
				// if closest intersection so far
				if(t0 < t){
					t = t0;
					return s;
				}
			}
			else{
				// if closest intersection so far
				if(t0 < t){
					t = t0;
					return level[i];
				}
			}
		}
	}
	return NULL;
}

RGB Scene::getLighting(ShadeRay& shadeRay, Point3D& hitLocation){
	// Equation 14.10
	// L0(p,wo) = ka*cd * (ls*cl) + sumj[(kd*cd / pi) * (ls,j*cl,j) * (n.lj)]
	// where
	//   L0 is total reflected radiance at point p
	//   ka is ambient reflection coefficient (which == kd (kd is diffuse reflection coefficient) for diffuse)
	//   cd is diffuse colour of object
	//   ls is radiance scaling factor of ambient light (ls,j is each other light)
	//   cl is colour of ambient light (cl,j is colour of each other light)
	//   n is normal of surface hit location

	RGB L0 = shadeRay.material->getAmbientShading(shadeRay, *ambientLight);
	// initial sum just has ambient light, switched round equation to simplify so that we always have RGB * float rather than float * RGB

	// check contribution from each (directional) light in the scene
	for(int i=0; i<lightList.size(); i++){

		double t0;
		Vector v = Vector(lightList[i]->position - shadeRay.intersectionPoint); // vector from hit point to light source
		double distance = (sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2)));
		Vector v0 = v / distance; // normalised
		Ray shadowRay = Ray(shadeRay.intersectionPoint, v0);
		if(shadowIntersection(shadowRay,t0,distance)){
			// in shadow, so don't add lighting effects of this light
		}
		else{ // not in shadow
			// shading depends on light AND material, so must get material first, therefore must be done in another function
			lightList[i]->direction = lightList[i]->getDirection(shadeRay.intersectionPoint); // only changes point lights, everything else just returns: lightList[i]->direction
			
			L0 = L0 + shadeRay.material->shadeSurface(shadeRay, *lightList[i], L0);
		}
	}
	L0 = L0 + shadeRay.material->reflection(shadeRay);
	return L0;
}

void Scene::addLight(Lights* lightPtr) {  
	lightList.push_back(lightPtr);
}

void Scene::constructLights(void){
	ambientLight = new AmbientLight(0.7);

	PointLight* pointLight = new PointLight(Point3D(300,-400,-100),0.8,RGB(255.0/255.0,255.0/255.0,255.0/255.0));
	addLight(pointLight);

	pointLight = new PointLight(Point3D(-400,-100,-500),0.5,RGB(20.0/255.0,255.0/255.0,255.0/255.0));
	addLight(pointLight);

	//from camera eye
	/*pointLight = new PointLight(Point3D(0,-100,-400),Vector(0,-1,0),1);
	addLight(pointLight);*/

	//from behind scene
	/*pointLight = new PointLight(Point3D(0,-100,600),Vector(0,-1,0),1);
	addLight(pointLight);*/
}

void Scene::constructObjects(void){
	//default scene
	Plane* plane = new Plane(Point3D(0,0,0),Normal(0,-1,0));
	plane->baseColour = RGB(200.0/255.0,100.0/255.0,100.0/255.0);
	plane->materialPtr = new Lambert();
	addShape(plane);
	
	Sphere* sphere = new Sphere(Point3D(-200,-80,85),90);
	sphere->baseColour = RGB(100.0/255.0,200.0/255.0,100.0/255.0);
	sphere->materialPtr = new Lambert();
	addShape(sphere);

	sphere = new Sphere(Point3D(0,-50,0),150);
	sphere->baseColour = RGB(80.0/255.0,80.0/255.0,200.0/255.0);
	sphere->materialPtr = new Specular();
	addShape(sphere);

	sphere = new Sphere(Point3D(25,-220,0),20);
	sphere->baseColour = RGB(255.0/255.0,20.0/255.0,147.0/255.0);
	sphere->materialPtr = new Specular();
	addShape(sphere);

	sphere = new Sphere(Point3D(-25,-220,0),20);
	sphere->baseColour = RGB(240.0/255.0,240.0/255.0,10.0/255.0);
	sphere->materialPtr = new Specular();
	addShape(sphere);

	Triangle* triangle = new Triangle(Point3D(-100,0,0),Point3D(-100,-280,0),Point3D(100,0,0));
	triangle->baseColour = RGB(30.0/255.0,100.0/255.0,50.0/255.0);
	triangle->materialPtr = new Lambert();
	addShape(triangle);

	triangle = new Triangle(Point3D(100,0,0),Point3D(100,-280,0),Point3D(-100,0,0));
	triangle->baseColour = RGB(30.0/255.0,100.0/255.0,50.0/255.0);
	triangle->materialPtr = new Lambert();
	addShape(triangle);

	vector<GeometricShape*> bbox0contents;
	{
		triangle = new Triangle(Point3D(60,-60,-200),Point3D(-40,0,-220),Point3D(0,-60,-180));
		triangle->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
		triangle->materialPtr = new Lambert();
		bbox0contents.push_back(triangle);

		/*for(int i=-10; i<10; i++){
			for(int j=-10; j<10; j++){
				sphere = new Sphere(Point3D(i*18,-10,-200 - i*2 + j*18),10);
				sphere->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
				if((i+j)%2==0) sphere->materialPtr = new Lambert();
				else sphere->materialPtr = new Specular();

				bbox0contents.push_back(sphere);
			}
		}*/

		// bbox inside bbox
		/*vector<GeometricShape*> bbox00contents;
		vector<GeometricShape*> bbox01contents;
		vector<GeometricShape*> bbox02contents;
		{
			for(int i=-1000; i<1000; i+=2){
				triangle = new Triangle(Point3D((rand() % 30) + i,(rand() % 100) - 110,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 110,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 110,(rand() % 100) + 800));
				triangle->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
				triangle->materialPtr = new Lambert();
				bbox00contents.push_back(triangle);

				triangle = new Triangle(Point3D((rand() % 30) + i,(rand() % 100) - 210,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 210,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 210,(rand() % 100) + 800));
				triangle->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
				triangle->materialPtr = new Specular();
				bbox01contents.push_back(triangle);

				triangle = new Triangle(Point3D((rand() % 30) + i,(rand() % 100) - 310,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 310,(rand() % 100) + 800),
					Point3D((rand() % 30) + i,(rand() % 100) - 310,(rand() % 100) + 800));
				triangle->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
				triangle->materialPtr = new Lambert();
				bbox02contents.push_back(triangle);
			}
			BoundingBox* bbox00 = new BoundingBox(bbox00contents);
			BoundingBox* bbox01 = new BoundingBox(bbox01contents);
			BoundingBox* bbox02 = new BoundingBox(bbox02contents);
			bbox0contents.push_back(bbox00);
			bbox0contents.push_back(bbox01);
			bbox0contents.push_back(bbox02);
		}*/
		
		triangle = new Triangle(Point3D(0,-10,-200),Point3D(-10,0,-200),Point3D(0,0,-200));
		triangle->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
		triangle->materialPtr = new Lambert();
		bbox0contents.push_back(triangle);
		
		BoundingBox* bbox0 = new BoundingBox(bbox0contents);
		addShape(bbox0);
	}

	// read in object(s) from file
/*	vector<Point3D> vertices;
	double scale = 120;
	string LINE;
	ifstream infile;
	infile.open ("vertices.txt");
    while(!infile.eof()) // loop through each line
    {
	    getline(infile,LINE); // read line into LINE
		string buf; 
		stringstream ss(LINE); // put line in a stream

		// stream values into a point
		double x,y,z;
		ss >> buf;
		stringstream(buf) >> x;
		ss >> buf;
		stringstream(buf) >> y;
		ss >> buf;
		stringstream(buf) >> z;

		// save point in list of all vertices
		vertices.push_back(Point3D(x*scale,y*scale,z*scale));
    }
	infile.close();

	// finally, we can loop through the vertices and assign triangles to them. Put them in a bounding box for speed
	vector<GeometricShape*> bbox1contents;
	{
		Triangle* face;
		for(int i=2; i<vertices.size(); i++){
			face = new Triangle(vertices[i-2],vertices[i-1],vertices[i]);
			face->baseColour = RGB(178.0 / 255.0, 58.0 / 255.0, 238.0 / 255.0);
			face->materialPtr = new Lambert();
			bbox1contents.push_back(face);
		}
		BoundingBox* bbox1 = new BoundingBox(bbox1contents);
		addShape(bbox1);
	}
	*/














//Spiral spheres scene
	/*double angle = 0;
	double pi = 3.141592653589793;
	double radius = 100;
	double mini_radius = 20;
	Sphere* sphere;
	double spheres_per_cycle = 14.0;
	double angle_inc = 365.0 / spheres_per_cycle;
	double depth_inc = (mini_radius * 2.0) / (spheres_per_cycle-1);
	vector<GeometricShape*> bbox0contents;
	vector<GeometricShape*> bbox1contents;
	vector<GeometricShape*> bbox2contents;
	vector<GeometricShape*> bbox3contents;
	int z = 50;
	while(radius>0){
		for(int xy=0; xy<spheres_per_cycle; xy++){
			angle = (double)((int)(angle + angle_inc) % 360);
			sphere = new Sphere(Point3D(cos(angle*pi/180)*radius,sin(angle*pi/180)*radius,z),mini_radius);
			sphere->baseColour = RGB((rand() % 256) / 255.0, (rand() % 256) / 255.0, (rand() % 256) / 255.0);
			sphere->materialPtr = new Specular();

			if(angle < 90) bbox0contents.push_back(sphere);
			else if(angle < 180) bbox1contents.push_back(sphere);
			else if(angle < 270) bbox2contents.push_back(sphere);
			else if(angle < 361) bbox3contents.push_back(sphere);

			z+=depth_inc;
			if(z>200)radius--;
		}
	}
	BoundingBox* bbox0 = new BoundingBox(bbox0contents);
	BoundingBox* bbox1 = new BoundingBox(bbox1contents);
	BoundingBox* bbox2 = new BoundingBox(bbox2contents);
	BoundingBox* bbox3 = new BoundingBox(bbox3contents);
	addShape(bbox0);
	addShape(bbox1);
	addShape(bbox2);
	addShape(bbox3);*/
}