#include "Scene.h"
#include "Hyperboloid.h"
#include "Cone.h"
#include "Plane.h"
#include <fstream>
#include <iostream>
using namespace std;

Scene::Scene(string filename)
{
	//open a file
	ifstream ifs(filename);


	string token; //for consuming file input

	//loop though the file
	while (ifs >> token)
	{
		//check to see what object must be read in
		if (token == "light_ambient")
		{
			ifs >> ambient;			// consume the given color
			ifs >> token;			// consume 'end'
		}
		else if (token == "light_source")
		{
			Light ptLight;
			ifs >> ptLight;			// get light source
			ifs >> token;			// consume 'end'

			lights.push_back(ptLight);
		}
		else if (token == "camera")
		{
			ifs >> cam;				// get camera
			ifs >> token;			// consume 'end'
		}
		else if (token == "movie")
		{
			ifs >> token >> frames;				// read in details for animation
			ifs >> token >> radius;
			ifs >> token >> height;	
			ifs >> token;						// consume 'end'

			if (frames > 900) frames = 900;		//cap number of frames to be 900	

		}
		else if (token == "sphere")
		{
			Sphere* sphere = new Sphere();
			ifs >> *sphere;			// get sphere

			shapes.push_back(sphere);
		}
		else if (token == "cylinder")
		{
			Cylinder* cylinder = new Cylinder;
			ifs >> *cylinder;		// get cylinder

			shapes.push_back(cylinder);
		}
		else if (token == "hyperboloid")
		{
			Hyperboloid* hyper = new Hyperboloid;
			ifs >> *hyper;			// get hyperboloid

			shapes.push_back(hyper);
		}
		else if (token == "cone")
		{
			Cone* cone = new Cone;
			ifs >> *cone;			// get cone

			shapes.push_back(cone);
		}
		else if (token == "mesh")
		{
			Mesh* mesh = new Mesh;
			ifs >> *mesh;			// get mesh

			shapes.push_back(mesh);
		}
		else if (token == "plane")
		{
			Plane* plane = new Plane;
			ifs >> *plane;

			shapes.push_back(plane);
		}
	}
}

void Scene::render(string filename) const
{
	//make canvas
	Image canvas(cam.samples, cam.samples);

	float deltaW;		// amount of movement in window per one increase in x or y coordinate in the picture (canvas), which increases linearly
	float halfView = cam.view_size / 2;

	//calculate 'step' must take in window for every +1 in x or y in the canvas 
	float step = cam.view_size / cam.samples;
	float remainder = -halfView + ((cam.samples - 1) * step);		//only move samples - 1 number of steps, thus is some remainder to edge of window starting at x = -half_view
	remainder = halfView - remainder;
	remainder = remainder / (cam.samples - 1);
	deltaW = step + remainder;


	//for each point p in view window: shoot ray from the camera's eye to point p in window
	int canvasX = 0;
	int canvasY = 0;												// coordinates in canvas

	//compute reference point UL of the window
	Point UL = cam.C - halfView * cam.W + halfView * cam.Up;		//upper left corner of window


	for (float y = 0; y <= cam.samples-1; y++)						// take samples-1 num steps in image, so for sample size of 2, take one step and have pixel on LH side and RH side, then take one step down and have same again
	{
		for (float x = 0; x <= cam.samples-1; x++)
		{
			//Compute point p where will shoot ray at from the camera's position
			Point P = UL + (x * deltaW) * cam.W - (y * deltaW) * cam.Up;		//top left of window is (-cam.view_size/2, cam.view_size/2)

			Ray ray(cam.pos, P);

			//get color
			//(calculate intersection if exists, if so find direction vectors, then use illumination model)
			Color pixColor = this->trace(ray, cam.depth);
			canvas.setPixel(canvasX, canvasY, pixColor);

			canvasX++;		//go over one column in canvas
		}
		canvasX = 0;		//reset canvasX when shift down a row
		canvasY++;			//go down one row in canvas
	}


	//save to file
	canvas.saveImage(filename);

}

Color Scene::trace(const Ray& ray, int depth) const
{
	optional<Hit> hit = this->findClosestHit(ray, shapes);

	if (!hit)
	{
		return BLACK;
	}


	//if ray intersects with any shape, calculate color at intersection
	Color obColor = hit->color;
	Material obMat = hit->mat;

	Color color = obMat.ka * ambient * obColor;			//add ambient color to total color only once


	//general directions for this intersection
	Unit N = hit->normal;								//normal vector of intersection
	Unit V(-ray.dir());									//unit vector from intersect to 'eye' (simply reverse of given ray)
	 
	//if angle between N and V is greater than or equal to 90, we have ambient color only 
	if (dot(N, V) < 0) return color;


	//Sum of intensity from point lights
	for (const Light& light : lights)
	{
		//find directions unique to each light
		Unit L = Unit(hit->inter, light.pos);			//unit vector to point light source
		Unit R = 2 * (dot(N, L) * N) - L;				//unit vector representing reflection of point light source


		//calculate color for this light
		if (dot(N, L) > 0) {																			//only if N*L > 0 (light visible) do we check if light is blocked

			//see if visible light gets blocked by a shape
			Vector shadowVector(hit->inter, light.pos);													//non-normalized vector from intersection to light source
			float maxDist = shadowVector.length();														//maxDist (t to light source). 
			Ray shadowRay(hit->inter, shadowVector);													//shadow ray from point of intersect to light source

			optional<Hit> blockingHit = this->findClosestHit(shadowRay, shapes, maxDist);
			if (!blockingHit)																			//light visible and not blocked? Use Illumination EQ
			{
				Color diffComp = obMat.kd * light.color * obColor * (dot(N, L));

				Color specComp = BLACK;
				if (dot(R, V) > 0) {																	//specular computed only if angle between R and V is less than 90
					specComp = obMat.ks * light.color * obColor * pow(dot(R, V), obMat.n);
				}

				color = color + diffComp + specComp;
			}
		}
	}
	


	//reflection 
	//base case: return color from max 'depth' reflection, break chain if hit object is not reflective
	if (depth == 1 || obMat.ks == 0) return color;

	//else, object is reflective and depth still allows for another level of reflection
	Unit Vr = 2 * (dot(N, V) * N) - V;
	Ray reflectedRay(hit->inter, Vr);

	return color + (obMat.ks * this->trace(reflectedRay, depth - 1));
}

optional<Hit> Scene::findClosestHit(const Ray& ray, const vector<Shape*>& shapes, float maxDist) const
{
	optional<Hit> hit;			//to hold hit with lowest t (closest to camera/viewer)
	float minT = maxDist;

	// for each shape calculate hit (if exists)
	for (Shape* shape : shapes)
	{
		optional<Hit> temp = shape->intersect(ray);

		if (!temp) continue;								//no hit (intersect method removes case of object hitting self and blocking light to itself)

		if ((temp->t <= maxDist) && (temp->t <= minT))		//new minimum distance hit
		{
			minT = temp->t;
			hit = temp;
		}
	}

	return hit;
}

void Scene::animate(string moviePath)
{
	//compute radians per step
	float theta = (2 * PI) / (frames - 1);			// note: want to divide circle by frames-1 so that start and end in same place (full circle)

	string filename = moviePath + "100.ppm";		//start every animation at 100.ppm file

	//for each frame of animation, move around in circular motion
	for (int f = 1; f <= frames; f++)
	{
		//render scene at this position
		render(filename);	

		//prep for next frame
		float newX = cam.pos.z() * sin(theta) + cam.pos.x() * cos(theta);
		//y stays the same since rotating around y axis
		float newZ = cam.pos.z() * cos(theta) - cam.pos.x() * sin(theta);

		Point newPos(newX, cam.pos.y(), newZ);

		//update camera
		cam.lookAt(newPos, Point(0,0,0), Unit(0,1,0));				//assuming center of scene is (0,0,0) and Up vector is just y-axis

		//get next frame's file path ready
		filename = moviePath + to_string(100 + f) + ".ppm";
	}

}

void Scene::animate_spherical(string moviePath)
{
	//compute radians per step
	float theta = (2 * PI) / (frames - 1);			// note: want to divide circle by frames-1 so that start and end in similar x,z place
	float phi = (-PI / 3) / (frames - 1);			// note: phi in range [PI/2 ... PI/6] since are climbing up sphere from xz plane: [90..30] => [0..-60]

	string filename;

	//for each frame of animation, move around in spiral motion
	for (int f = 0; f < frames; f++)
	{
		//prep for this frame
		float currTheta = theta * f;				//keep track as go around
		float currPhi = (phi * f) + (PI/2);			// Note: spiral path starts at PHI = 90, so have to decrement FROM 90 to get the currPhi to go up toward the north pole

		float newX = radius * sin(currTheta) * sin(currPhi);
		float newY = radius * cos(currPhi);
		float newZ = radius * cos(currTheta) * sin(currPhi);

		Point newPos(newX, newY, newZ);

		//update camera
		//for spherical animation, Up vector is just the V vector from sphere bump mapping
		Vector N(Point(0, 0, 0), newPos);
		Vector U = Vector(0, 1, 0).cross(N);
		Vector Up = N.cross(U);

		cam.lookAt(newPos, Point(0, 0, 0), Unit(Up));				//assuming center of scene is (0,0,0)

		//get this frame's file path 
		filename = moviePath + to_string(100 + f) + ".ppm";

		//render scene at this position
		render(filename);
	}
}

void Scene::animate_cylindrical(string moviePath)
{
	//compute radians per step
	float theta = (2 * PI) / (frames - 1);			// note: want to divide circle by frames-1 so that start and end in similar x,z place

	string filename;

	//for each frame of animation, move around in spiral motion
	for (int f = 0; f < frames; f++)
	{
		//prep for this frame
		float currTheta = theta * f;				//keep track as go around

		float newX = radius * sin(currTheta);
		float newY = height * (1 - cos(currTheta)) / 2;
		float newZ = radius * cos(currTheta);

		Point newPos(newX, newY, newZ);

		//update camera
		float centerX = ((-radius / 10) * sin(currTheta + PI) * cos(currTheta + PI)) / (1 + pow(sin(currTheta + PI), 2));
		//float centerY = 0;
		float centerZ = ((radius / 10) * sin(currTheta + PI)) / (1 + pow(sin(currTheta + PI), 2));

		cam.lookAt(newPos, Point(centerX, 0, centerZ), Unit(0,1,0));				//for cylindrical animation, 'Up vector' is always aligned with y-axis, but is not true Up vector, camera must recompute
	

		//get this frame's file path 
		filename = moviePath + to_string(100 + f) + ".ppm";

		//render scene at this position
		render(filename);
	}
}


