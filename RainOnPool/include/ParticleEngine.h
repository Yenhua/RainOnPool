/*
 * ParticalEngine.h
 * This collects/stores all the information on raindrops and displays them
 * It will also modify/step through each raindrop position
 *  Created on: 5/10/2014
 *      Author: Yen-hua Chuang
 */

#pragma once

#include "define.h"
#include "G308_Geometry.h"
#include <GL/glut.h>

class ParticleEngine
{
private:
	//GLuint textureId;//may texture raindrops, not sure yet
	Rain rainDrops[1000];//1000 for now
	G308_Geometry rainDropsOBJ[1000];
	float timeUntilNextStep;


	void step();
	void initRainDrop(Rain* r);
	void initRainDropOBJ(G308_Geometry* r);
	void resetRainDropOBJ(G308_Geometry* r);


public:
	float NUM_PARTICLE;
	float dim;//for n by n square of possible values for raindrops to appear
	float height;//starting position for raindrops
	int quadrent;
	ParticleEngine(float n, float h);
	ParticleEngine(void);
	void draw();
	void advance(float t);

	float randomFloat();
	void setNumParticles(float n);
	void addParticles(float n);
	void removeParticles(float n);
};
