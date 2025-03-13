#include "Material.h"
#include <string>
using namespace std;

// Display material in format: 'Mat(ka, kd, ks, n)'
ostream& operator<<(ostream& os, const Material& mat) 
{
	cout << "Mat(" << mat.ka << "," << mat.kd << "," << mat.ks << "," << mat.n << ")";
	
	return os;
}

//
istream& operator>>(istream& is, Material& mat)
{
	string word;

	is >> word >> mat.ka;	// set ambient coeff
	is >> word >> mat.kd;	// etc..
	is >> word >> mat.ks;
	is >> word >> mat.n;	// set Phong size 

	is >> word;			// consume 'end'

	return is;
}