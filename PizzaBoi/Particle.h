#ifndef __Particle_H__
#define __Particle_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "Polyhedron.h"
#include "Texture.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class Particle: public Drawable{


public:
	Particle(); 
	Particle(int size); 
	~Particle();

	void loadObj(string filename, float scale);
	void draw(Camera, vector<Light*>);
	void init();
	void updateTexture(Texture t);
	void idle();

private:
	vector<vec4> colors;
	vector<vec4> positions;
	vector<vec4> velocities;
	vector<float> mass;
	int size;
	float speed;

	void collision(int n);

	GLuint vPosition;
	GLuint vColor;
	GLuint vVelocity;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	
};
#endif