/*
 * ParticalEngine.cpp
 *
 *  Created on: 5/10/2014
 *      Author: Yen-hua Chuang
 */


#include <stdio.h>
#include "define.h"
#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>
#include "ParticleEngine.h"
#include "define.h"






/**
 * (old) Constructor for partical engine
 */
ParticleEngine::ParticleEngine(){

	NUM_PARTICLE = 1000;//load 1000 and read them in, but never allow it to go over 1000
	timeUntilNextStep = 0;
	for(int i = 0; i < NUM_PARTICLE; i++) {


		//printf("float i is : %f\n",i);
		initRainDropOBJ(rainDropsOBJ + i);
	}
	NUM_PARTICLE = 100;
}

ParticleEngine::ParticleEngine(float n, float h){
	NUM_PARTICLE = 1000;
	dim = n;
	height = h;
	timeUntilNextStep = 0;

	quadrent = 0;
	int count = 0;

	for(int i = 0; i < NUM_PARTICLE; i++) {
		//printf("float i is : %f\n",i);
		initRainDropOBJ(rainDropsOBJ + i);
		count++;
		if(count>NUM_PARTICLE/4.0f){
			quadrent++;
			count = 0;
		}
	}

	NUM_PARTICLE = 200;//start of the number of particles
}


void ParticleEngine::initRainDropOBJ(G308_Geometry *r){
	if(quadrent==0){
			r->x = dim * randomFloat() - 0.25f;
			r->z = dim * randomFloat() - 0.25f;
	}
	if(quadrent==1){
				r->x = -1.0f*(dim * randomFloat() - 0.25f);
				r->z = dim * randomFloat() - 0.25f;
	}
	if(quadrent==2){
			r->x = dim * randomFloat() - 0.25f;
			r->z = -1.0f*(dim * randomFloat() - 0.25f);
	}
	if(quadrent==3){
			r->x = -1.0f*(dim * randomFloat() - 0.25f);
			r->z = -1.0f*(dim * randomFloat() - 0.25f);
	}
	r->y = (10.0f * randomFloat() - 0.25f)+height;//make them all start at 10 for now

//	r->x = 0;
//	r->y = 0;//make them all start at 10 for now
//	r->z = 0;

	float PI = 3.1415926535897932384626433832795;
	float projectedArea;
	float waterDensity = 999.97;
	float dragCoefficient = 0.47;
	float radius = 0.005; // (5mm)
	float gravity = 9.81;
	//TODO weight and velocity
	r->volume =  randomFloat() * 5.0;
	projectedArea = PI*(radius*radius);
	r->mass = r->volume * waterDensity;

	r->velocity = -1.0f*sqrt((2*r->mass*gravity)/(waterDensity*dragCoefficient*projectedArea));

	r->velocity = r->velocity*0.1f;
	//printf("terminal velocity is: %f\n",r->velocity);
	r->t = 0;
	r->ReadOBJ("DoubleConeStreak.obj");
	r->Translate(r->x,r->y,r->z);
}

void ParticleEngine::resetRainDropOBJ(G308_Geometry *r){
	float q = randomFloat();
	//printf("q is %f\n",q);
	if(q<0.25f){
		r->x = dim * randomFloat() - 0.25f;
		r->z = dim * randomFloat() - 0.25f;
	}
	else if(q>=0.25f && q<0.50f){
		r->x = -1.0f*(dim * randomFloat() - 0.25f);
		r->z = dim * randomFloat() - 0.25f;
	}
	else if(q>=0.50f && q<0.75f){
		r->x = dim * randomFloat() - 0.25f;
		r->z = -1.0f*(dim * randomFloat() - 0.25f);
	}
	else if(q>=0.75f){
		r->x = -1.0f*(dim * randomFloat() - 0.25f);
		r->z = -1.0f*(dim * randomFloat() - 0.25f);
	}
	r->y = (10.0f * randomFloat() - 0.25f)+height;//make them all start at 10 for now
	float PI = 3.1415926535897932384626433832795;
	float projectedArea;
	float waterDensity = 999.97;
	float dragCoefficient = 0.47;
	float radius = 0.005; // (5mm)
	float gravity = 9.81;
	//TODO weight and velocity
	r->volume =  (randomFloat() * 5.0)+1;
	projectedArea = PI*(radius*radius);
	r->mass = r->volume * waterDensity;

	r->velocity = -1.0f*sqrt((2*r->mass*gravity)/(waterDensity*dragCoefficient*projectedArea));
	r->velocity = (r->velocity*0.1f)+1;
	//printf("terminal velocity is: %f\n",r->velocity);

	r->t = 0;
	r->Translate(r->x,r->y,r->z);
}

/**
 * Advance the particle by STEP_TIME
 */
void ParticleEngine::step(){
	for(int i = 0; i < NUM_PARTICLE; i++) {
		G308_Geometry* g = rainDropsOBJ + i;
		g->y = g->y + (g->velocity*STEP_TIME);
		g->Translate(g->x,g->y,g->z);
		//printf("x pos is : %f\n",g->z);
		if (g->y < 0) {//has "collided" with the floor, this may need changing
			//TODO pool add wave (x,z)
			//pool->isCollision(x, y, z)
			resetRainDropOBJ(g);//make a new one
		}
	}
}

/**
 * This just calls step() for the required amount of time
 */
void ParticleEngine::advance(float t){
	while (t > 0) {
		if (timeUntilNextStep < t) {
			t  = t - timeUntilNextStep;
			step();
			timeUntilNextStep = STEP_TIME;
		}
		else {
			timeUntilNextStep -= t;
			t = 0;
		}
	}//end while
}

/**
 * Draw all the individual rain drops
 */
void ParticleEngine::draw(){

		for(int i = 0; i < NUM_PARTICLE; i++) {

			G308_Geometry* g = rainDropsOBJ + i;
			g->RenderGeometry();

		 }
}

/**
 * Returns a random float between 0 and 1
 */
float ParticleEngine::randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}

void ParticleEngine::setNumParticles(float n){
	if(n>999){
		return;
	}
	NUM_PARTICLE = n;
}

void ParticleEngine::addParticles(float n){
	if(n>999){
		return;
	}
	NUM_PARTICLE = NUM_PARTICLE +  n;
}
void ParticleEngine::removeParticles(float n){
	NUM_PARTICLE = NUM_PARTICLE -  n;
	if(NUM_PARTICLE<0){
		NUM_PARTICLE = 0;
	}
}
