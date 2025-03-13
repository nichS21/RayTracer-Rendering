#ifndef CONE_H
#define CONE_H

#include "Shape.h"


class Cone : public Shape
{
private:
	Point peak;		// top of cone
	float rx;		// radius in x axis
	float rz;		// radius in z axis
	float height;
public:
	/*
	* Return C++ 'optional' of 'Hit' object representing
	* the point of intersection between the shape and
	* a given ray.
	*/
	optional<Hit> intersect(const Ray& ray) const override;

	/*
	* Method to determine if given t results in a viable hit.
	* Returns nullopt if not viable
	*/
	optional<Hit> viableT(float t, const Ray& ray) const override;

	/*
	* Override from Shape.h
	*/
	pair<float, float> texel_uv(const Point& pt) const override;

	/*
	* Override from Shape.h
	*/
	Unit bumpNormal(const Point& pt, const Unit& N, float u, float v) const override;
	
	/*
	* Override from Shape.h
	*/
	Color selectColor(float u, float v) const;

	/*
	* Overide from Shape.h
	*/
	bool isVisible(float u, float v) const;

	/*
	* Overload of cout with access to private data members.
	*/
	friend ostream& operator<<(ostream& os, const Cone& c);

	/*
	* Overload of cin with access to private data members.
	* Note: assume keyword for class has already been consumed when this method is called,
	* within Scene class
	*/
	friend istream& operator>>(istream& is, Cone& c);
};

#endif