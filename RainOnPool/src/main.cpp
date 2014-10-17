//---------------------------------------------------------------------------
//
// Copyright (c) 2012 Taehyun Rhee
//
// This software is provided 'as-is' for assignment of COMP308 
// in ECS, Victoria University of Wellington, 
// without any express or implied warranty. 
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>

// My definition 
#include "define.h"
#include "G308_Geometry.h"
#include <iostream> 

#include "G308_ImageLoader.h"
#include "string.h"

#include "ParticleEngine.h"

using namespace std;

// Global Variables
GLuint g_mainWnd;
GLuint g_nWinWidth  = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

G308_Geometry* table = NULL;
G308_Geometry* singleRainOBJ = NULL;

Rain* singleRain = NULL;
bool animating = false;

ParticleEngine *rainGenerator;
const int TIMER_MS = 25; //The number of milliseconds to which the timer is set

bool scaled = false;
//in this one, we will need to initialize a textureinfo for each texture/object

void G308_Display() ;
void G308_Reshape(int w, int h) ;
void G308_SetCamera();
void G308_SetLight();
void G308_keyboardListener(unsigned char, int, int);
void renderTable();
void renderRain();
void Timer(int value);
void setMaterial();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("COMP308 GP rain");

    glutDisplayFunc(G308_Display);
    glutReshapeFunc(G308_Reshape);
    //glutMouseFunc(G308_ChangeFrame);
    glutKeyboardFunc(G308_keyboardListener);

	table = new G308_Geometry;
	table->ReadOBJ("Table.obj"); // 1) read OBJ function
	table->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
	table->CreateGLWireGeometry(); // do we still need this?


	//rainGenerator = new ParticleEngine();
	rainGenerator = new ParticleEngine(30.0f,10.0f);



	G308_SetCamera();
	G308_SetLight();

	glutTimerFunc(TIMER_MS, Timer, 0);
	glutMainLoop();

	if(table != NULL) delete table;
    return 0;
}

// Display function
void G308_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);


	//can scale the table in x and z to make it bigger
	//remember to keep y as 1!
	renderTable();
	glColor3f(169.0/255.0,197.0/255.0,200.0/255.0);

	setMaterial();

	rainGenerator->draw();
	//singleRainOBJ->RenderGeometry();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}
// Reshape function
void G308_Reshape(int w, int h)
{
    if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;
    
    glViewport(0, 0, g_nWinWidth, g_nWinHeight);  
}

// Set Light
void G308_SetLight(){
	GLfloat ambient_color[] = {0.5f, 0.5f, 0.5f, 1.0f};//increase color components and it becomes bright
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

	glShadeModel (GL_SMOOTH);
	glEnable(GL_LIGHT0);
}

// Set Camera Position
void G308_SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 20.0, 50.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);

}

void setMaterial(){
	//glColor3f(1,0,0);
	glColor3f(169.0/255.0,197.0/255.0,200.0/255.0);
}

void G308_keyboardListener(unsigned char key, int x, int y) {
	//Code to respond to key events
	if(key=='p'){
		Timer(0);
	}
	if(key=='q'){
		printf("q is pressed\n");
	}
	if(key=='d'){
		rainGenerator->setNumParticles(20.0f);
		printf("light drizzle is pressed\n");
	}
	if(key=='x'){
		rainGenerator->setNumParticles(0.0f);
		printf("x is pressed, no rain\n");
	}
	if(key=='p'){
		rainGenerator->addParticles(10.0f);
		printf("p is pressed, adding 10\n");
		printf("Number of raindrops are now: %f\n",rainGenerator->NUM_PARTICLE);
	}
	if(key=='o'){
		rainGenerator->removeParticles(10.0f);
		printf("o is pressed, removing 10\n");
		printf("Number of raindrops are now: %f\n",rainGenerator->NUM_PARTICLE);
	}
	if(key=='s'){
		rainGenerator->setNumParticles(1.0f);
		printf("s is pressed, setting to single raindrop\n");
	}

}

/**
 * Repeat the update/adance function for the particle engine
 */
void Timer(int value){
	rainGenerator->advance(TIMER_MS / 1000.0f);

    glutPostRedisplay();
    // 100 milliseconds
    glutTimerFunc(TIMER_MS, Timer, 0);
}


/**
 * I'm planning on updating and rendering a whole collection of rain to act as
 * a partical system. For now lets just get a single rain drop moving
 */
void renderRain(){
	glColor3f(0.0,0.0,0.5);
	glPushMatrix();
	glScalef(0.5,1.0,0.5);
	glTranslatef(singleRain->x,singleRain->y,singleRain->z);
	glutSolidSphere(0.4, 500, 500);
	glPopMatrix();

}


/**
 * Table from Assignment 3, for now I'm just  using it as the floor
 */
void renderTable(){
	glColor3f(0.0,0.1,0.0);//green table for now TODO textures
	glPushMatrix();
	glScalef(1,1,1.5);
	table->RenderGeometry();
	glPopMatrix();
}


