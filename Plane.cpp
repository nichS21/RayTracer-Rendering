#include "Plane.h"


optional<Hit> Plane::intersect(const Ray& ray) const
{
	//See notes for full work
	float t = (dot(-normal, Vector(ray.origin())) - dist) / (dot(normal, ray.dir()));

	return viableT(t, ray);
}

optional<Hit> Plane::viableT(float t, const Ray& ray) const
{
	//negative t (or t too close to object) = immediate fail
	if (t < ZERO) return {};

	//check to see if ray is perpendicular to plane's normal (i.e., no intersection)
	if (dot(ray.dir(), normal) == 0) return {};

	//otherwise t must be valid 
	Point intersect = ray.point(t);

	//check if object isVisible (for masking purposes)
	auto [u, v] = texel_uv(intersect);

	if (isVisible(u, v))
	{
		//get either solid color of shape or color of its texture at intersection
		Color obColor = selectColor(u, v);


		Unit bumpNorm = bumpNormal(intersect, normal, u, v);						// if bump map for this shape, perturb normal, otherwise normal remains the same

		// create Hit object and return it
		return Hit{ intersect,bumpNorm,obColor,t,mat };
	}

	return {};																		//intersection is not visible -> do not return a hit
}

pair<float, float> Plane::texel_uv(const Point& pt) const
{
	//compute Q, U, and V to help find u,v coords for texture mapping
	Vector yAxis(0, 1, 0);
	Vector U = yAxis.cross(normal);									// U always perp to y axis and normal (right hand rule; very similar to reasoning for sphere)

	if (U == Vector(0, 0, 0)) U = Vector(1, 0, 0).cross(normal);	// if yAxis and N are parallel dot product is zero, so use x axis instead, like with mesh

	Vector V = normal.cross(U);

	Point Q = Vector(dist * -normal).point();						// given distance from origin, travel along negative normal to reach plane [Q is within plane]

	//compute u, v; note that neither can be made into range [0..1], are arbitrarily large
	Vector QP(Q, pt);
	float u = dot(QP, U);
	float v = dot(QP, V);

	return pair<float, float> {u, v};
}

Unit Plane::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;								//no bump map, return regular normal


	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient_wh(u, v);						//use gradient_wh for direct coordinates only with plane
	Vector yAxis(0, 1, 0);
	Vector U = yAxis.cross(N);										// U perp to y axis and normal (right hand rule; similar to reasoning for sphere) with added case (next line)

	if (U == Vector(0, 0, 0)) U = Vector(1, 0, 0).cross(N);			// if yAxis and N are parallel dot product is zero, so use x axis instead, like with mesh

	Vector V = N.cross(U);

	Vector norm = N + du * U + dv * V;


	return Unit(norm);
}


Color Plane::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_wh(u, v);
}


bool Plane::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_wh(u, v) > 0;
}


ostream& operator<<(ostream& os, const Plane& p)
{
	cout << "Plane: " << p.normal << " Dist (d):" << p.dist << " " << p.mat;

	return os;
}


istream& operator>>(istream& is, Plane& p)
{
	Point normPt;

	is >> normPt >> p.dist;		//read in normal and distance along normal to reach plane

	p.normal = Unit(normPt);

	p.readApperance(is);

	return is;
}