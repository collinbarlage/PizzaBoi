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
	Object(string n);
	~Object();

	void draw(Camera, vector<Light*>);
	void makeTexture(char * name, int width, int height);

private:
	Polyhedron * poly;
	vector<Polyhedron*> polyhedrons;
	void makePizza();
	void makeHouse();
};
#endif