#include "Object.h"


Object::Object(float r) {
	seed = r;
	//Loads all textures for objects
	tPizzaPep = Texture("./objects/pizzaPep.ppm", 1280, 720);
	tPizzaCheese = Texture("./objects/pizzaCheese.ppm", 1280, 720);
	tPizzaSpecial = Texture("./objects/pizzaSpecial.ppm", 1280, 720);
	tPizzaCrust = Texture("./objects/pizzaCrust.ppm", 1280, 720);
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

void Object::makePizza(mat4 m) {
	polyhedrons.clear();
	//Topping
	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaTop.obj", .2);
	
	switch(random(3)) {
	case 1:	
		poly->textureInit(tPizzaPep);
		break;
	case 2:	
		poly->textureInit(tPizzaCheese);
		break;
	case 3:	
		poly->textureInit(tPizzaSpecial);
		break;
	}
	polyhedrons.push_back(poly);
	//Crust
	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaCrust.obj", .2);
	poly->textureInit(tPizzaCrust);
	polyhedrons.push_back(poly);
	//Bottom
	poly = new Polyhedron();
	poly->loadObj("./objects/pizzaBottom.obj", .2);

	poly->textureInit(tPizzaCrust);

	polyhedrons.push_back(poly);


	transform(m);
}

void Object::makeHouse(mat4 m) {
	//TODO: model a haus!
	//...


	transform(m);
}

int Object::random(int p) {
	int r = int(((float) rand() / RAND_MAX *p))+1;
	cout << r << endl;
	return r;
}

void Object::updateTexture(Texture t) {
	//abstract
}
