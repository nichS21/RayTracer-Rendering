#ifndef TEST_RAYTRACER_
#define TEST_RAYTRACER_

#include <string>
#include <vector>
using namespace std;

// given a directory with .txt scene files and a list of scene names
// (without .txt extension) saves a rendering of the scenes in .ppm files
//
// for example: test_raytracer("part1", {"scene1", "scene2", "scene3"});

void test_raytracer(const string& dir, const vector<string>& namesList);

#endif