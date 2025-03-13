#ifndef CYLINDER_H
#define CYLINDER_H

#include "Point.h"
#include "Material.h"
#include "Color.h"
#include "Hit.h"
#include "Ray.h"
#include "Shape.h"
#include <optional>


class Cylinder : public Shape
{
private:
	Point center;
	float height;
	float radius;

public:
	/*
	* Return C++ 'optional' of 'Hit' object representing
	* the point of intersection between the cylinder and
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
	* Overload of cout for Cylinder, with access to private data members.
	* Format: 'Cylinder: Point(1,2,3); Height:456; Radius:12; Color(r, g, b); Mat(ka,kd,ks,n)'
	*/
	friend ostream& operator<<(ostream& os, const Cylinder& c);

	/*
	* Overload of cin for Cylinder, with access to private data members.
	* Note: assume 'cylinder' word has already been consumed when this method is called,
	* within Scene class
	*/
	friend istream& operator>>(istream& is, Cylinder& c);

};


#endif