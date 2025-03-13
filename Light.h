#ifndef LIGHT_H
#define LIGHT_H

#include "Point.h"
#include "Color.h"
#include <iostream>
using namespace std;

struct Light
{
	//Data members
	Point pos;
	Color color;
};

//cout overload
ostream& operator<<(ostream& os, const Light& light);

//cin overload
istream& operator>>(istream& is, Light& light);



#endif