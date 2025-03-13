#include "Shape.h"

void Shape::readApperance(istream& is)
{
	string token;
	bool colorTexture = false;						// shape may have only solid color OR texture

	
	while (is >> token)
	{
		if (token == "end") break;					// read 'end' -> are done
		else if (token == "solid" && !colorTexture)
		{
			is >> this->color;						//shape has color only
			colorTexture = true;
		}
		else if (token == "texture" && !colorTexture)
		{
			is >> token;
			this->texture = new Image(token);		// shape has texture map only (get path to texture file)
			colorTexture = true;
		}
		else if (token == "material")
		{
			is >> this->mat;						// shape's material properties
		}
		else if (token == "bump_map")
		{
			is >> token;
			this->bumpMap = new Image(token);		// shape's bump map texture
		}
		else if (token == "mask")
		{
			is >> token;
			this->mask = new Image(token);			// shape's mask texture
		}
	}
}

pair<float, float> Shape::quadratic(double a, double b, double c) const
{

	float t1, t2;
	double discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0)			// no real roots, return intentionally non-valid t's to get rejected
	{
		return pair<float, float> {-1, -1};
	}
	
	// two real roots (also includes case of one root, but just send it as two values)
	discriminant = sqrt(discriminant);
	t1 = (-b + discriminant) / (2 * a);
	t2 = (-b - discriminant) / (2 * a);

	if (t2 < t1) return pair<float, float> {t2, t1};
	

	return pair<float, float> {t1, t2};
}




