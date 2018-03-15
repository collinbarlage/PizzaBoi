#include "Object.h"


Object::Object(string n, mat4 m, float r) {
	if(!n.compare("pizza")) {
		makePizza();
	} else if(!n.compare("house")) {
		//etc
	}
	//translate
	for(int i=0; i<polyhedrons.size(); i++) {
		polyhedrons[i]->setModelMatrix(m);
	}
	//randomize
	seed = r;
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

void Object::transform(mat4 m) {
	for(int i=0; i<polyhedrons.size(); i++) {
		polyhedrons[i]->setModelMatrix(m);
	}
}

void Object::makePizza() {
	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaPie.obj", 1);
	switch(random(3)) {
	case 1:	
		poly->textureInit("./objects/pizzaCheese.ppm", 1280, 720);
		break;
	case 2:	
		poly->textureInit("./objects/pizzaPep.ppm", 1280, 720);
		break;
	case 3:	
		poly->textureInit("./objects/pizzaSpecial.ppm", 1280, 720);

		break;
	}
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


int Object::random(int p) {
	int r = int(((float) rand() / RAND_MAX *p))+1;
	cout << r << endl;
	return r;
}