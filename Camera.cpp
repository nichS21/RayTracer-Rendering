#include "Camera.h"

void Camera::lookAt(Point eye, Point center, Unit up)
{
	//update camera position (eye), center of scene, and up vector
	pos = eye;
	C = center;
	Up = up;

	//recompute V and W
	V = Unit(pos, C);
	W = V.cross(Up);

	//recompute true Up vector (mainly for cylindrical animation)
	Up = W.cross(V);
}

ostream& operator<<(ostream& os, const Camera& cam) 
{
	cout << "Camera: " << cam.pos << " " << cam.view_size << "cm " << cam.samples << "px Depth"  << cam.depth 
		<< " C: " << cam.C << " Up: " << cam.Up << " V: " << cam.V << " W: " << cam.W;

	return os;
}

istream& operator>>(istream& is, Camera& cam)
{
	string word;

	is >> word >> cam.pos;			// consume 'pos', create point
	is >> word >> cam.view_size;	
	is >> word >> cam.samples;
	is >> word >> cam.depth;
	is >> word >> cam.C;			// consume 'look_at' and get C (center of scene)
	is >> word >> cam.Up;			// consume 'sky' and get Up vector for camera

	//compute animation related components
	cam.V = Unit(cam.pos, cam.C);
	cam.W = cam.V.cross(cam.Up);


	return is;
}