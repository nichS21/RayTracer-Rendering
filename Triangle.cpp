#include "Triangle.h"

optional<Hit> Triangle::intersect(const Ray& ray) const
{
	//implementing EQ 6 from the linked paper
	Vector v0(vertices[0]);
	Vector v1(vertices[1]);
	Vector v2(vertices[2]);
	Vector e1 = v1 - v0;
	Vector e2 = v2 - v0;
	Vector P = ray.dir().cross(e2);
	Vector T(v0.point(), ray.origin());
	Vector Q = T.cross(e1);

	Vector rHand(dot(Q, e2), dot(P, T), dot(Q, ray.dir()));

	if (dot(P, e1) == 0) return {};						// Case 0: ray is parallel to the triangle, this intersection is not valid (analgous to disciminant = 0 in quadratic equation)

	float lHand = 1 / dot(P, e1);

	Vector result = lHand * rHand;						// result vector containing: t, u, v in that order
	float t = result.x();

	//check if t is viable
	if (t < ZERO) return{};								// Case 1: negative t (or t too close to object) = immediate fail
	
	float u = result.y();
	float v = result.z();
	float w = 1 - u - v;
	if (u >= 0 && v >= 0 && 0 <= w)						// Case 2: plane of triangle could be hit, but triangle itself is missed [Note: 'u+v <= 1' equivalent to '0 <= w']
	{
		// calculate intersect using t
		Point intersect = ray.point(t);

		//calculate weighted average of color
		Color weigColor = w * vColors[0] + u * vColors[1] + v * vColors[2];

		//calculate weighted normal
		Unit weigNorm = w * vNormals[0] + u * vNormals[1] + v * vNormals[2];

		return Hit{ intersect, weigNorm, weigColor, t, {}, u, v };
	}
	
	// triangle missed, even though plane hit
	return {};
}

void Triangle::setSmooth(bool smooth)
{
	// loop over each vertex and set color at that vertex
	for (int i = 0; i < 3; i++)
	{
		Unit uVertex(vertices[i]);
		Color vertColor(abs(uVertex.x()), abs(uVertex.y()), abs(uVertex.z()));

		vColors[i] = vertColor;
	}
	
	if(!smooth)															// flat triangle -> each vertex's normal is the triangle's flat normal
	{
		for (int i = 0; i < 3; i++) vNormals[i] = flatNorm;
	}
	
}

void Triangle::resize(float scale, const Vector& translate)
{
	for (int i = 0; i < 3; i++)
	{
		Vector newPos = (scale * vertices[i]) + translate;
		vertices[i] = newPos.point();
	}
}

ostream& operator<<(ostream& os, const Triangle& t)
{
	cout << "Triangle: " << t.vertices[0] << " " << t.vNormals[0] << " "
						 << t.vertices[1] << " " << t.vNormals[1] << " "
						 << t.vertices[2] << " " << t.vNormals[2] << " "
						 << t.color << " " << t.flatNorm;

	return os;
}

istream& operator>>(istream& is, Triangle& t)
{
	string token; 
	Point vertPt;
	Point normPt;


	for (int i = 0; i < 3; i++)
	{
		is >> vertPt >> normPt;					// get vertex, normal pair
		t.vertices[i] = vertPt;
		t.vNormals[i] = Unit(Vector(normPt));
	}

	//compute flat normal given the vertices (cross product counter clockwise)
	Vector v1V2 = Vector(t.vertices[0], t.vertices[1]);
	Vector v1V3 = Vector(t.vertices[0], t.vertices[2]);

	t.flatNorm = Unit(v1V2.cross(v1V3));
	

	is >> t.color;								// read in triangle's color			


	/*
	* By default triangles vertex colors are the color of the whole triangle
	*/
	for (int i = 0; i < 3; i++)
	{
		t.vColors[i] = t.color;
	}


	return is;
}