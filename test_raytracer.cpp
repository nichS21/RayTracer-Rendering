#include "test_raytracer.h"
#include "Scene.h"

#include <string>
#include <vector>
using namespace std;

void test_raytracer(const string& dir, const vector<string>& namesList)
{
	for (const string& name : namesList) {
		//Scene(dir + "/" + name + ".txt").render(dir + "/" + name + ".ppm");			//render single scene
		//Scene(dir + "/" + name + ".txt").animate(dir + "/");							//render planar animation -> go around origin in circular pattern
		//Scene(dir + "/" + name + ".txt").animate_spherical(dir + "/");				//render spherical animation -> from xz plane, go in spiral motion up the sphere
		Scene(dir + "/" + name + ".txt").animate_cylindrical(dir + "/");				//render cylindrical animation -> travel along elliptical path around curve of a cylinder

			
	}
}