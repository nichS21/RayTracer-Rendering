# RayTracer

This is an archive of a raytracer rendering program for Intro to Computer Graphics at my undergraduate university. All files used to render scenes on the accompanying site are included in this repo under the '*part__*' directories. The program is capable of rendering individual scenes, as well as multiple scenes in succession for planar, spherical, and cylindrical animation.

Visit my gallery site [here.](http://www.cs.gettysburg.edu/~stacni01/cs373/) Homeworks 2-6 showcase this RayTracer program.


**Please note:** the accompanying '.jar' application was written by Professor [Ivalyo Ilinkin](https://www.gettysburg.edu/academic-programs/computer-science/faculty/employee_detail.dot?empId=04006600720013294&pageTitle=Ivaylo+Ilinkin) of Gettysburg College, I only kept it here as it is a useful tool to view *.PPM* files on Windows and it can create an *APNG* file from multiple rendered *.PPM* files, making an 'animated' scene. The same applies for all models and maps, these were provided by Professor Ilinkin as handouts.


## Setup (easiest way)
Using Visual Studio 2022, make sure 'vcpkg,'  a C++ dependency manager, is installed with your Visual Studio. Then:

1)  Open a terminal within Visual Studio
2)  type '*vcpkg new --application*'
3)  then '*vcpkg add port freeglut*'

You should now be able to build and run the application.
