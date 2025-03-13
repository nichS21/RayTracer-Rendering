#include "Point.h"

#include <iostream>
using namespace std;

Point::Point(float x, float y, float z)
	:
	x_(x), y_(y), z_(z)
{
}

float Point::x() const
{
	return x_;
}

float Point::y() const
{
	return y_;
}

float Point::z() const
{
	return z_;
}

//Format: 'Point(1, 2, 3)'
ostream& operator<<(ostream& os, const Point& pt)
{
	os << "Point(" << pt.x() << ", " << pt.y() << ", " << pt.z() << ")";
	return os;
}

//Format: '<1, 2, 3>'
istream& operator>>(istream& is, Point& pt)
{
	char ch;
	float x, y, z;

	is >> ch;		// read '<'
	is >> x >> ch;	// read coord and comma
	is >> y >> ch;
	is >> z;
	is >> ch;		// read '>'

	pt = Point(x, y, z); // make the point from user input

	return is;
}

