#ifndef __Object_H__
#define __Object_H__

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

class Object: public Drawable{


public:
	Object(float r=(float) rand() / (RAND_MAX)); //Loads all textures for objects
	~Object();

	void draw(Camera, vector<Light*>);
	void updateTexture(Texture t);
	void transform(mat4 m);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat theta);

	void spawn(GLfloat x, GLfloat y, GLfloat z);
	void setAnimation(vec3 movePath, GLfloat spin);
	void stopAnimation();
	void animate(); //x,y,z is translation, a is angle of rotation

	void makePizza(GLfloat=1, GLfloat=1, GLfloat=1);
	void makeHouse(GLfloat=1, GLfloat=1, GLfloat=1);
	void makeSkull(GLfloat=1, GLfloat=1, GLfloat=1);

	vec3 getLocation();

private:
	Polyhedron * poly;
	vector<Polyhedron*> polyhedrons;
	int random(int p);
	float seed;

	bool isAnimating;
	vec3 movePath;
	GLfloat spin;

	//keep track of objects location
	GLfloat x;
	GLfloat y;
	GLfloat z;

	
};
#endif