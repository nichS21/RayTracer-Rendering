#ifndef CAMERA_H
#define CAMERA_H

#include "Point.h"
#include "Unit.h"


struct Camera
{
	//Data members
	Point pos;			//position of camera in world
	float view_size;	//area in world where camera is focused
	float samples;		//num samples horizontally and vertically (image size)
	int depth;			//num levels of reflection to have

	//for animation:
	Point C;			//point for camera to 'look at'
	Unit Up;			//up vector of camera
	Unit V;				//view vector
	Unit W;				//perpendicular to Up and the view vector, V

	
	//Method that updates internal state of camera, for when it moves during animation rendering
	void lookAt(Point eye, Point center, Unit up);
};

//overload cout, format: 'Camera: Point(x,y,z) 60cm 300px'
ostream& operator<<(ostream& os, const Camera& cam);

//overload cin to read a camera object
istream& operator>>(istream& is, Camera& cam);


#endif