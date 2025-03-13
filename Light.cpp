#include "Light.h"

#include <iostream>
#include <string>
using namespace std;

// Format: "Light: Point(1, 2, 3), Color(1, 1, 1)"
ostream& operator<<(ostream& os, const Light& light)
{
	cout << "Light: " << light.pos << ", " << light.color;

	return os;
}

istream& operator>>(istream& is, Light& light)
{

	Point position;
	Color color;

	string word;

	is >> word;		//consume 'pos'
	is >> position; //get point
	is >> color;	//get color

	light.pos = position;
	light.color = color;

	return is;
}