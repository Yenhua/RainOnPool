Readme for COMP308 Assignment 1
ID:300263763
Name:Yen-hua Chuang

Run instructions:
Navigate to the appropriate directory with the terminal, 308A1_Core or 308A1_Extra

In 308A1_Core:
make clean
make
./build/Ass1 assets/teapot.obj

These are the required program arguments, but it will always just draw the default glut teapot.
Clicking on the screen can toggle it between smooth shading and wireframes.

In 308A1_Extra:
make clean
make
./build/Ass1 assets/teapot.obj or ./build/Ass1 assets/bunny_new.obj

The name of the .obj file can be replaced with others, like the Shere.
The dragon does not work.
It may be hard to tell if the wireframes are drawn appropriately for the bunny
since it's zoomed out. I haven't implemented any camera controls.
Uncommenting out //gluLookAt(0.0, 2.5, 20.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0); zooms in a bit
and makes the wireframe clearer to see.




Description of what I've done:
Core:
Compiling the source code in the ECS machine can be done by navigating to the directory Makefile
on the console and use the following commands:
"make clean" then "make"

I work on a Windows 7 laptop, so I only focused on getting the program to compile and run on
Eclipse, then I would copy the source file onto the ECS machines to verify it does work as
they will be marked on the ECS machines.

Steps to get it working on Windows:
I did follow the steps from the tutorial from the Lecture Schedule at:
http://ecs.victoria.ac.nz/foswiki/pub/Courses/COMP308_2014T2/LectureSchedule/COMP308_EclipseCppWinGuide.pdf

Quick SUmmary:
1)Install Eclipse for C/C++
2)Install minGW and MSYS
3)Download freeglut for MinGW and moved the files into the appropriate folders

After everything has been installed and set up properly:
4)Open Eclipse and import the project from the assignment page
5)Go to project properties and under C/C++ Build, check "Generate MakeFile automatically"
This enables "Tool Settings" tab to show in Settings
6)Then in Tool Settings, in Libraries under MinGW C++ Linker, the following libraries needed to be added:
opengl32
glut32
glu32

Then by building the project and running, the default teapot should work.

I do not modify the Makefile as I would just compile it on the ECS machines before I submit the assignment.

Extra:
I only managed to complete:
-allow loading and display of other files (Sphere and Teapot) 
-load the bunny.obj file

(I did not manage to get the Dragon to render)

To render each .obj, I iterate through all the faces stored in m_pTriangles (Each G308_Triangle is a face).
The information here are just indices for the actual vertex co-ordinates, so I just access the
co-ordinates from m_pVertexArray and m_pNormalArray and use them in glVertex3f and glNormal3f
to render the model.

To render the bunny, I modified readOBJ so that if numUV == 0. we'll use this to parse the information:
sscanf(str, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
and that'll parse the information without the texture co-ordinates.
Then we can render the bunny as usual by accessing the actual vertex values from the indices.


