#include "Hyperboloid.h"
#include "utils.h"
#include <numbers>

optional<Hit> Hyperboloid::intersect(const Ray& ray) const 
{
	//solve for t using equation for this shape (plug in ray parametric EQ, then rearrange to make a quadratic)
	float h = max(ha, hb);

	/*double a = ((ray.dir().x() * ray.dir().x()) / (rx*rx)) -
			   ((ray.dir().y() * ray.dir().y()) / (h*h)) +
			   ((ray.dir().z() * ray.dir().z()) / (rz * rz));*/

	/*double b = (((2 * ray.origin().x() * ray.dir().x()) - (2 * ray.dir().x() * center.x())) / (rx * rx)) -
			(((2 * ray.origin().y() * ray.dir().y()) - (2 * ray.dir().y() * center.y())) / (h*h)) +
			(((2 * ray.origin().z() * ray.dir().z()) - (2 * ray.dir().z() * center.z()))	/ (rz*rz));*/

	/*double c = ((pow(ray.origin().x(), 2) + pow(center.x(), 2) - (2 * ray.origin().x() * center.x())) / (rx*rx)) -
				((pow(ray.origin().y(), 2) + pow(center.y(), 2) - (2 * ray.origin().y() * center.y())) / (h*h)) +
				((pow(ray.origin().z(), 2) + pow(center.z(), 2) - (2 * ray.origin().z() * center.z())) / (rz*rz)) -
				1;*/
	//Same math above is achieved below through vectors and 'dot products' instead

	Vector dirSkewed(ray.dir().x() / rx, ray.dir().y() / h, ray.dir().z() / rz);

	double a = (dirSkewed.x() * dirSkewed.x()) -				//take 'dot product' of dirSkewed, but we have to manually account for the minus in the middle
		       (dirSkewed.y() * dirSkewed.y()) +
		       (dirSkewed.z() * dirSkewed.z());

	Vector oriCenSkewed((ray.origin().x() - center.x()) / rx,
					   (ray.origin().y() - center.y()) / h,
					   (ray.origin().z() - center.z()) / rz);

	double b = 2 * ((dirSkewed.x() * oriCenSkewed.x()) -
				   (dirSkewed.y() * oriCenSkewed.y()) +
				   (dirSkewed.z() * oriCenSkewed.z()));

	double c = ((oriCenSkewed.x() * oriCenSkewed.x()) -
			    (oriCenSkewed.y() * oriCenSkewed.y()) +
			    (oriCenSkewed.z() * oriCenSkewed.z()))
			    -1;

	//calculate discriminant (get t values if are any)
	pair<float, float> tValues = quadratic(a, b, c);


	//go through t's, and return first (closest) valid hit
	optional<Hit> hit = viableT(tValues.first, ray);
	if (hit) return hit;								//first hit valid, return


	return viableT(tValues.second, ray);				//second hit either valid or invalid
}

optional<Hit> Hyperboloid::viableT(float t, const Ray& ray) const
{
	//negative t (or t too close to object) = immediate fail
	if (t < ZERO) return {};

	//calculate intersection
	Point intersect = ray.point(t);

	//check to make sure intersection happens within height of hyperboloid, and return nullopt if doesn't
	if (intersect.y() > center.y() + ha || intersect.y() < center.y() - hb) return {};


	//check if object isVisible (for masking purposes) at this point, else is black color
	auto [u, v] = texel_uv(intersect);

	if (isVisible(u, v))
	{
		//get either solid color of shape or color of it's texture at intersection
		Color obColor = selectColor(u, v);

		//compute normal (must get gradient vector of hyperboloid and plug in intersect's points)
		//check notes for full math
		float h = max(ha, hb);
		float x = (2 * (intersect.x() - center.x())) / (rx * rx);
		float y = -(2 * (intersect.y() - center.y())) / (h * h);
		float z = (2 * (intersect.z() - center.z())) / (rz * rz);

		Unit normal(x, y, z);
		normal = bumpNormal(intersect, normal, u, v);						// if bump map for this shape, perturb normal, otherwise normal remains the same


		// create Hit object and return it
		return Hit{ intersect,normal, obColor,t,mat };
	}

	return {};																// intersection is not visible, do not return a hit
}


pair<float, float> Hyperboloid::texel_uv(const Point& pt) const
{
	// correct point to be at origin
	Point corrP(pt.x() - center.x(), pt.y() - center.y(), pt.z() - center.z());

	//see math in notes
	float theta = atan2(rz*corrP.x(), rx*corrP.z());				// range: [-PI..PI]
	//and already know 'y'											// range: [-Hb..Ha]

	//convert both to range [0..1]
	float u = (theta + PI) / (2 * PI);								//now in range [0..1]
	float v = ( -corrP.y() + hb) / (ha + hb);						// note: have to invert y for similar reason to cylinder

	return pair<float, float> {u, v};
}

Unit Hyperboloid::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;		//no bump map, return regular normal

	//else was a bump map
	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient(u, v);

	Vector yAxis(0, 1, 0);
	Vector U(-pt.z(), 0, pt.x());			//Note: hyperboloid normal calculated to have 0 as y component, thus have just x and z components -> swap the two and negate one to get perp (dot product = 0)
	Vector V = N.cross(U);					//cross of N and U is V (right hand rule)
			
	Vector norm = N + du * U + dv * V;
	return Unit(norm);
}

Color Hyperboloid::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_uv(u, v);
}

bool Hyperboloid::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_uv(u, v) > 0;
}

ostream& operator<<(ostream& os, const Hyperboloid& h)
{
	cout << "Hyperboloid: " << h.center << " Rx: " << h.rx << " Rz: " << h.rz
		 << " Ha: " << h.ha << " Hb: " << h.hb << "\n" << h.color
		 << " " << h.mat;

	return os;
}


istream& operator>>(istream& is, Hyperboloid& h)
{
	string word;

	is >> word >> h.center;			// get center point
	is >> word >> h.rx >> h.rz;		// get radius for x and z
	is >> h.ha >> h.hb;				// get height above and below

	h.readApperance(is);			// get appearance of shape


	return is;
}