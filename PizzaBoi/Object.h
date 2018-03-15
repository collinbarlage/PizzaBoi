#ifndef __Object_H__
#define __Object_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "Polyhedron.h"
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
	Object(string, mat4=Scale(1,1,1), float r=(float) rand() / (RAND_MAX));
	~Object();

	void draw(Camera, vector<Light*>);
	void makeTexture(char * name, int width, int height);
	void transform(mat4 m);

private:
	Polyhedron * poly;
	vector<Polyhedron*> polyhedrons;
	void makePizza();
	void makeHouse();
	int random(int p);
	float seed;
};
#endif