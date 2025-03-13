// #include "test_geomtypes.h"
#include "test_raytracer.h"
#include "Image.h"

int main()
{
	//test_geomtypes();

	// Render all scenes for gallery 
	//test_raytracer("part2", { "scene6", "scene7", "scene8", "scene9", "scene10", });
	
	//test_raytracer("part3", { "scene1", "scene2", "scene3", "scene4", "scene5", "scene5-1", "scene6", "scene7", "scene8"});
	
	//test_raytracer("part4", { "bunnyNoMapFlat", "bunnyNoMap", "magNoMap", "magNoMapFlat", "catBump", "pencilBump", "dolphinMask", "roseTex"});
	//test_raytracer("part4", { "catBumpS", "pencilBumpS", "dolphinMaskS", "roseTexS" });

	//test_raytracer("part5/movie4", { "movie4"});									//change for movie1 - 4, can also change animation type in 'test_raytracer.cpp'



	// Testing scenes
	// Note: to change between static render and specific animation type, must edit 'test_raytracer.cpp'
	test_raytracer("part5/movie4", { "movie4"});


	return 0;
}