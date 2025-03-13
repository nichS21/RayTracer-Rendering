#include "Cylinder.h"
#include "utils.h"
#include <numbers>


optional<Hit> Cylinder::intersect(const Ray& ray) const
{

	//solve for t using equation for cylinder, similar to way done with Sphere class
	Vector rayVector(ray.dir().x(), 0, ray.dir().z());
	double a = dot(rayVector, rayVector);


	Vector centerOrigin(ray.origin().x() - center.x(),			//represent ray origin - center as a vector
						0,
						ray.origin().z() - center.z());
	double b = dot(2 * rayVector, centerOrigin);


	double c = dot(centerOrigin, centerOrigin) - radius * radius;

	//calculate discriminant (get t values if are any)
	pair<float, float> tValues = quadratic(a, b, c);


	//go through t's, and return first (closest) valid hit
	optional<Hit> hit = viableT(tValues.first, ray);
	if (hit) return hit;								//first hit valid, return


	return viableT(tValues.second, ray);				//second hit either valid or invalid
}


optional<Hit> Cylinder::viableT(float t, const Ray& ray) const
{
	//negative t (or t too close to object) = immediate fail
	if (t < ZERO) return {};

	//calculate intersection
	Point intersect = ray.point(t);

	//check to make sure intersection happens within height of cylinder, and return nullopt if doesn't
	float halfH = height / 2;
	if (intersect.y() > center.y() + halfH || intersect.y() < center.y() - halfH) return {};


	//check if object isVisible (for masking purposes) at this point, else is black color
	auto [u, v] = texel_uv(intersect);

	if (isVisible(u, v))
	{
		//get either solid color of shape or color of it's texture at intersection
		Color obColor = selectColor(u, v);

		//compute normal
		Point adjustedCenter(center.x(), intersect.y(), center.z());		// 'raise' center to level of intersection, because we don't want to include the y component in the normal, else we have a sphere

		Unit normal = Unit(adjustedCenter, intersect);
		normal = bumpNormal(intersect, normal, u, v);						// if bump map for this shape, perturb normal, otherwise normal remains the same

		// create Hit object and return it
		return Hit{ intersect,normal,obColor,t,mat };
	}

	return {};																// intersection is not visible, do not return a hit
}


pair<float, float> Cylinder::texel_uv(const Point& pt) const
{
	// correct point to be at origin
	Point corrP(pt.x() - center.x(), pt.y() - center.y(), pt.z() - center.z());

	float theta = atan2(corrP.x(), corrP.z());		// range: [-PI, PI]
	//and already know 'y'							// range: [-height/2, height/2]

	//convert both to range [0..1]
	float u = (theta + PI) / (2*PI); 
	float v = (-corrP.y() + (height / 2)) / height;									//note: must invert y to ensure [+h/2] (top of cylinder) maps to 0 (top of image) and [-h/2] maps to 1 (bottom of image)

	return pair<float, float> {u, v};
}

Unit Cylinder::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;		//no bump map, return regular normal

	//else was a bump map
	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient(u, v);

	Vector V(0, 1, 0);						// V is the y-axis, in the case of a cylinder
	Vector U(pt.z(), 0, -pt.x());			//Note: cylinder normal is same regardless of y level (y component is 0 in normal), thus have just x and z components -> swap the two and negate one to get perp (dot product = 0)

	Vector norm = N + du*U + dv*V;
	return Unit(norm);
}

Color Cylinder::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_uv(u, v);
}

bool Cylinder::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_uv(u, v) > 0;
}

ostream& operator<<(ostream& os, const Cylinder& c)
{
	cout << "Cylinder: " << c.center << "; " << c.radius << "; " << c.height << "; " << c.color << "; " << c.mat;

	return os;
}


istream& operator>>(istream& is, Cylinder& c)
{
	string word;

	//read in from file
	is >> word >> c.center;					// consume 'peak' and create peak point
	is >> word >> c.radius >> c.height;		// consume 'size,' then get radius and height
	
	c.readApperance(is);					// get appearance of shape
		

	return is;

}