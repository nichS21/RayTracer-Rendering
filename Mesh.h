#ifndef MESH_H
#define MESH_H

#include "Shape.h"
#include "Triangle.h"
#include "Sphere.h"
#include <vector>

class Mesh : public Shape
{
private:
	vector<Triangle> triangles;			//triangles that make up the mesh
	Sphere bound;						// bounding sphere
	string mapMode;						// mapMode: 'direct' mapping, 'spherical' mapping, or 'none'

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
	* Method that takes a given hit and updates it based on the mapping mode for the shape.
	* Example: spherical mapping must have a hit on bounding sphere, so extend hit on triangle, till hits sphere, then return that hit instead.
	*/
	optional<Hit> updateHit(optional<Hit> hit) const;

	/*
	* Override from Shape.h
	*/
	Color selectColor(float u, float v) const;

	/*
	* Overide from Shape.h
	*/
	bool isVisible(float u, float v) const;

	/*
	* Overload of cout for Sphere, with access to private data members.
	* Format: 'Sphere: Point(1,2,3) 456 Mat(ka,kd,ks,n)'
	*/
	friend ostream& operator<<(ostream& os, const Mesh& m);

	/*
	* Overload of cin for Sphere, with access to private data members.
	* Note: assume 'sphere' word has already been consumed when this method is called,
	* within Scene class
	*/
	friend istream& operator>>(istream& is, Mesh& m);
};

#endif
