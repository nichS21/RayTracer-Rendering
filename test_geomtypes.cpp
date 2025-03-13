#include "test_geomtypes.h"
#include "utils.h"
#include "Point.h"
#include "Vector.h"
#include "Unit.h"
#include "Ray.h"
#include "Light.h"
#include "Scene.h"

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

void test_geomtypes()
{
	//**Point methods/functions**//
	cout << "---Testing Point class---" << endl;

	Point p1(5, 4, 9);
	Point p2(2, 8, 12);

	assert(Point() == Point(0, 0, 0));
	assert(!(p1 == p2));

	assert(p1.x() == 5);
	assert(p1.x() != 9999);
	assert(p1.y() == 4);
	assert(p1.y() != 1001);
	assert(p1.z() == 9);
	assert(p1.z() != 4040);

	cout << "\nPoint p1: " << p1 << " [expect 'Point(5, 4, 9)']." << endl;
	cout << "\nPoint p2: " << p2 << " [expect 'Point(2, 8, 12)']." << endl;

	cout << "\nEnter '<[float], [float], [float]>' to create a point.";
	Point p3;
	cin >> p3;	// enter <[float], [float], [float]> to create a point

	cout << "Point you entered: " << p3 << endl;


	//**Vector methods/functions**//
	cout << "\n\n---Testing Vector class---" << endl;

	//Test constructors
	Vector v1(2.5, 1.55, 3);
	Vector v2(5, 6, 7);
	Vector v3(2, 3, 4);

	assert(Vector() == Vector(0, 0, 0));
	assert(Vector(p1, p2) == Vector(-3, 4, 3));
	assert(!(v2 == v3));

	//Test accessors
	assert(eq_zero((v1.x() - 2.5)));
	assert(v2.x() != 3000);
	assert(eq_zero(v1.y() - (float)1.55));
	assert(v2.y() != 6777);
	assert(v1.z() == 3);
	assert(v2.z() != 7999);

	//Test length functions
	assert(eq_zero(v2.length() - (float)sqrt(110)));
	assert(eq_zero(v2.sq_length() - 110));

	//Test scalar product
	assert(v2 * 2 == Vector(10, 12, 14));
	assert(v2 * 3 == 3 * v2);

	//Test division by scalar
	assert(Vector(6, 4, 2) / 2 == Vector(3, 2, 1));
	assert(v1 / 1 == v1);

	//Test dot product
	assert(eq_zero(dot(v2, v3) - 56));

	//Test vector addition
	assert(v2 + v3 == Vector(7, 9, 11));
	assert(v3 + v2 == Vector(7, 9, 11));

	//Test vector subtraction
	assert(v2 - v3 == Vector(3, 3, 3));
	assert(v3 - v2 == Vector(-3, -3, -3));

	//Test vector inversion
	assert(-v1 == Vector(-2.5, -1.55, -3));
	assert(-(-v1) == v1);
	cout << "\n" << -v2 << " [expect 'Vector(-5, -6, -7)']" << endl;

	//Test point add/subtract
	assert(p2 + v2 == Point(7, 14, 19));
	assert(p1 - v1 == Point(2.5, 2.45, 6));
	
	//These operations are not supported (as intended),
	// uncomment to see.
	//Point wrongAdd = v2 + p2;
	//Point wrongSub = v1 - p1;

	//Test cout overload
	Vector v4 = v3 - Vector(1, 2, 3);
	v4 = -v4;
	v4 = v4 + Vector(1, 1, 1);
	cout << "\n" << v4 << " [expect 'Vector(0, 0, 0)']" << endl;
	v4 = Vector(2, 2, 2);
	v4 = v4 * 2;
	v4 = v4 / 4;
	cout << v4 << " [expect 'Vector(1, 1, 1)']" << endl;


	//**Unit methods/functions**//
	cout << "\n\n---Testing Unit class---" << endl;

	//Test constructors
	Unit u1(1, 2, 3);					//vector has length sqrt(14)
	assert(eq_zero(u1.x() - (1 / (float)sqrt(14))));
	assert(eq_zero(u1.y() - (2 / (float)sqrt(14))));
	assert(eq_zero(u1.z() - (3 / (float)sqrt(14))));

	Unit u2(v3);						//<2, 3, 4> == length sqrt(29)
	assert(eq_zero(u2.x() - (2 / (float)sqrt(29))));
	assert(eq_zero(u2.y() - (3 / (float)sqrt(29))));
	assert(eq_zero(u2.z() - (4 / (float)sqrt(29))));

	Unit u3(p1, p2);					//<-3, 4, 3> has length = sqrt(34)
	assert(eq_zero(u3.x() - (-3 / (float)sqrt(34))));
	assert(eq_zero(u3.y() - (4 / (float)sqrt(34))));
	assert(eq_zero(u3.z() - (3 / (float)sqrt(34))));

	//Test cout overload
	cout << "\nu1: " << u1 << " [expect 'Unit(1/sqrt(14), 2/sqrt(14), 3/sqrt(14)']" << endl;
	cout << "\nu3: " << u3 << " [expect 'Unit(-3/sqrt(34), 4/sqrt(34), 3/sqrt(34)']" << endl;


	//**Ray methods/functions**//
	cout << "\n\n---Testing Ray class---" << endl;

	//Test constuctors, accessors, and cout overload
	Ray r1(p2, v3);
	assert(r1.origin() == Point(2, 8, 12));
	assert(r1.dir() == Unit(2, 3, 4));		//coordinates of v3
	cout << r1 << " [expect 'Point(2, 8, 12), Unit(2/sqrt(29), 3/sqrt(29), 4/sqrt(29)']" << endl;

	Ray r2(Point(1, 2, 3), Point(4, 5, 6));  //gives a vector <3, 3, 3>
	assert(r2.origin() == Point(1, 2, 3));
	assert(r2.dir() == Unit(3, 3, 3));
	cout << "\n " << r2 << " [expect 'Point(1, 2, 3), Unit(3/sqrt(27), 3/sqrt(27), 3/sqrt(27)']" << endl;

	//test point method
	Ray r3(Point(0, 0, 0), Vector(0, 1, 0));
	Point ptTest = r3.point(4);
	assert(ptTest == Point(0, 4, 0));


	//**Light methods/functions**//
	cout << "\n\n---Testing Light class---" << endl;

	//Manually make a point to test cout
	Light light1;
	light1.color = Color(1, 4, 5);
	light1.pos = Point(1, 7, 7);

	cout << "\n" << light1 << " [expect 'Light: Point(1, 7, 7), Color(1, 4, 5)']" << endl;

	//Test cin overload
	Light light2;
	cout << "\nEnter a light object; format: 'light_source pos <x, y, z> rgb <r, g, b> end'";
	cin >> light2;
	cout << "\nLight source you entered: " << light2 << endl;


	//**Unit methods/functions**//
	cout << "\n\n---Testing Scene class---" << endl;

	//Test reading scene from a file
	Scene scene("part1/scene.txt");


	//**End of Testing**//
	cout << "\n\n////Tests completed succesfully.////" << endl;
}