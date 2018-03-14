#include "Object.h"


Object::Object(string n) {
	if(!n.compare("pizza")) {
		makePizza();
	} else if(!n.compare("house")) {
		//etc
	}

}


Object::~Object(){

}

//----------------------------------------------------------------------------

void Object::draw(Camera cam, vector<Light*> lights){
	for(int i=0; i<polyhedrons.size(); i++) {
		polyhedrons[i]->draw(cam, lights);
	}
}


//----------------------------------------------------------------------------

void Object::makePizza() {
	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaPie.obj", 1);
	poly->textureInit("./objects/pizzaPep.ppm", 1, 1);
	polyhedrons.push_back(poly);

	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaCrust.obj", 1);
	poly->textureInit("./objects/pizzaCrust.ppm", 1280, 720);
	polyhedrons.push_back(poly);

	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaDough.obj", 1);
	poly->textureInit("./objects/pizzaCrust.ppm", 1280, 720);
	polyhedrons.push_back(poly);
}

void Object::makeTexture(char * name, int width, int height) {
	//abstract
}


