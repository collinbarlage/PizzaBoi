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
	void makePizza(mat4=Scale(1,1,1));
	void makeHouse(mat4=Scale(1,1,1));

private:
	Polyhedron * poly;
	vector<Polyhedron*> polyhedrons;
	int random(int p);
	float seed;
	
	//Textures:
	Texture tPizzaPep;
	Texture tPizzaCheese;
	Texture tPizzaSpecial;
	Texture tPizzaCrust;
	
};
#endif