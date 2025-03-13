#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"


class Plane : public Shape
{
private:
	Unit normal;				//normal to the plane from the origin
	float dist;					//dist along normal to reach plane [pos dist means origin above plane -> on side of normal]

public:
	//Override from Shape.h. Compute intersection between plane and a given ray.
	optional<Hit> intersect(const Ray& ray) const;

	//Override from Shape.h
	optional<Hit> viableT(float t, const Ray& ray) const;

	//Override from Shape.h
	pair<float, float> texel_uv(const Point& pt) const;

	//Override from Shape.h
	Unit bumpNormal(const Point& pt, const Unit& N, float u, float v) const;

	//Override from Shape.h
	Color selectColor(float u, float v) const;

	//Override from Shape.h
	bool isVisible(float u, float v) const;

	//cout overload for plane
	friend ostream& operator<<(ostream& os, const Plane& p);

	//cin overload for plane
	friend istream& operator>>(istream& is, Plane& p);

};

#endif
