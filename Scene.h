#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Image.h"
#include "Cylinder.h"
#include "Shape.h"
#include "Mesh.h"
#include <string>
#include <vector>
using namespace std;


class Scene
{
private:
	Camera cam;
	Color ambient;				// representing ambient light
	vector<Light> lights;		// point lights
	vector<Shape*> shapes;		// shapes of scene

	//data members related to animation
	int frames;					// number of frames in animation
	float radius;				// radius of path of animation	(dome of spherical or radius for cylindrical
	float height;				// height of cylinder for cyndrical animation	

public:
	/*
	* Constructor, create a scene from a given file
	*/
	Scene(string filename);

	/*
	* Create a blank canvas, render the scene object to the canvas,
	* and save canvas as a PPM image with given filename
	*/
	void render(string filename) const;

	/*
	* Determines amount of intensity contributed by a given ray;
	* if the ray hits the shape calculates the color at the hit 
	* point using the illumination model.
	*/
	Color trace(const Ray& ray, int depth) const;

	/*
	* Method that takes a given ray and checks which hit (if any)
	* among all the shapes is the closest to the camera/viewer.
	* 
	* Optional parameter 'maxDist' specifies maxDistance to consider hits at,
	* default is maximum value of a float
	*/
	optional<Hit> findClosestHit(const Ray& ray, const vector<Shape*>& shapes, float maxDist = FLT_MAX) const;

	/*
	* Generates a full circle animation of the scene around the y axis.
	* Reads the scene file in the given directory and places the frames in the same directory
	*/
	void animate(string moviePath);

	/*
	* Animate a scene where the camera moves along a spiral path along the surface of a circle
	*/
	void animate_spherical(string moviePath);

	/*
	* Creates an animation going up the curve of a cylinder along an elliptical path
	*/
	void animate_cylindrical(string moviePath);

};

#endif

