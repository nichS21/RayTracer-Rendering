#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
using namespace std;

struct Material 
{
	//Data members
	float ka;			// ambient coeffiecient
	float kd;			// diffuse coeffiecient
	float ks;			// specular coefficient
	float n;			// specular spot size (Phong size)

	//overload << as a friend method
	friend ostream& operator<<(ostream& os, const Material& mat);
};

//overload >> as a function
istream& operator>>(istream& is, Material& mat);


#endif