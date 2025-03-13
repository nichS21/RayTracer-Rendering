#include "Cone.h"
#include "utils.h"
#include <iostream>
#include <numbers>

optional<Hit> Cone::intersect(const Ray& ray) const
{
	//solve for t using equation for this shape (plug in ray parametric EQ, then rearrange to make a quadratic)
	Vector dirSkewed(ray.dir().x() / rx, ray.dir().y() / height, ray.dir().z() / rz);

	double a = (dirSkewed.x() * dirSkewed.x()) -				//take 'dot product' of dirSkewed, but we have to manually account for the minus in the middle
			   (dirSkewed.y() * dirSkewed.y()) +
			   (dirSkewed.z() * dirSkewed.z());

	Vector oriCenSkewed((ray.origin().x() - peak.x()) / rx,
						(ray.origin().y() - peak.y()) / height,
						(ray.origin().z() - peak.z()) / rz);

	double b = 2 * ((dirSkewed.x() * oriCenSkewed.x()) -
					(dirSkewed.y() * oriCenSkewed.y()) +
					(dirSkewed.z() * oriCenSkewed.z()));

	double c = (oriCenSkewed.x() * oriCenSkewed.x()) -
			   (oriCenSkewed.y() * oriCenSkewed.y()) +
			   (oriCenSkewed.z() * oriCenSkewed.z());

	//calculate discriminant (get t values if are any)
	pair<float, float> tValues = quadratic(a, b, c);


	//go through t's, and return first (closest) valid hit
	optional<Hit> hit = viableT(tValues.first, ray);
	if (hit) return hit;								//first hit valid, return


	return viableT(tValues.second, ray);				//second hit either valid or invalid
}


optional<Hit> Cone::viableT(float t, const Ray& ray) const
{
	//negative t (or t too close to object) = immediate fail
	if (t < ZERO) return {};

	//calculate intersection
	Point intersect = ray.point(t);

	//check to make sure intersection happens within height of cone, and return nullopt if doesn't
	if (intersect.y() > peak.y() || intersect.y() < peak.y() - height) return {};


	//check if object isVisible (for masking purposes) at this point, else is black color
	auto [u, v] = texel_uv(intersect);

	if (isVisible(u, v))
	{
		//get either solid color of shape or color of it's texture at intersection
		Color obColor = selectColor(u, v);

		//compute normal (must get gradient vector and plug in intersect's points)
		//check notes for full math
		float x = (2 * (intersect.x() - peak.x())) / (rx * rx);
		float y = -(2 * (intersect.y() - peak.y())) / (height * height);
		float z = (2 * (intersect.z() - peak.z())) / (rz * rz);

		Unit normal(x, y, z);
		normal = bumpNormal(intersect, normal, u, v);						// if bump map for this shape, perturb normal, otherwise normal remains the same

		//create Hit object and return it
		return Hit{ intersect,normal,obColor,t,mat };
	}

	return {};																// intersection is not visible, do not return a hit
	
}

pair<float, float> Cone::texel_uv(const Point& pt) const
{
	// correct point to be at origin
	Point corrP(pt.x() - peak.x(), pt.y() - peak.y(), pt.z() - peak.z());

	//see math in notes (based on slides)
	float theta = atan2(rz*corrP.x(), rx*corrP.z());		// range: [-PI, PI]
	//and already know 'y'									// range: [-height, 0]

	//convert both to range [0..1]
	float u = (theta + PI) / (2 * PI);						//now in range [0..1]
	float v = -corrP.y() / height;							// note: have to invert y for similar reason to cylinder

	return pair<float, float> {u, v};
}

Unit Cone::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;		//no bump map, return regular normal

	//else was a bump map
	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient(u, v);

	Vector V(pt, peak);						// V always leads to the peak of cone
	Vector U = V.cross(N);					

	Vector norm = N + du * U + dv * V;
	return Unit(norm);
}

Color Cone::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_uv(u, v);
}

bool Cone::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_uv(u, v) > 0;
}

ostream& operator<<(ostream& os, const Cone& c)
{
	cout << "Cone: " << c.peak << " Rx: " << c.rx << " Rz: " << c.rz
		<< " Height: " << c.height << "\n" << c.color
		<< " " << c.mat;

	return os;
}


istream& operator>>(istream& is, Cone& c)
{
	string word;

	is >> word >> c.peak;			// get center point
	is >> word >> c.rx;				// get radius for x
	is >> c.height >> c.rz;			// get height and radius for z

	c.readApperance(is);			// get appearance of shape


	return is;
}