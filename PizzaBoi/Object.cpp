#include "Object.h"


Object::Object(float r) {
	seed = r;
	x = y = z = 0;
	isAnimating = false;
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

void Object::translate(GLfloat tX, GLfloat tY, GLfloat tZ) {
	for(int i=0; i<polyhedrons.size(); i++) {
		polyhedrons[i]->setModelMatrix(Translate(tX,tY,tZ));
	}
	x += tX;
	y += tY;
	z += tZ;
}

void Object::spawn(GLfloat sX, GLfloat sY, GLfloat sZ) {
	translate(-x,-y,-z);
	translate(sX,sY,sZ);
	x = sX;
	y = sY;
	z = sZ;
}

void Object::animate() {
	if(isAnimating) {
		for(int i=0; i<polyhedrons.size(); i++) {
			polyhedrons[i]->setModelMatrix(Translate(movePath.x,movePath.y,movePath.z));
			
			//TODO: rotation is so F'd. fixx dis shite
			//polyhedrons[i]->setModelMatrix(Translate(movePath.x,movePath.y,movePath.z)*Translate(x,y,z)*RotateY(spin)*Translate(-x,-y,-z));
		}
		x += movePath.x;
		y += movePath.y;
		z += movePath.z;

		//TODO: if object is super far away stop animating it
	}

}

void Object::setAnimation(vec3 v, GLfloat s) {
	movePath = v;
	spin = s;
	isAnimating = true;
}

void Object::stopAnimation() {
	isAnimating = false;
}

void Object::makePizza(GLfloat tX, GLfloat tY, GLfloat tZ) {
	polyhedrons.clear();
	//Load textures
	Texture tPizzaPep = Texture("./objects/pizza/pizzaPep.ppm", 1280, 720);
	Texture tPizzaCheese = Texture("./objects/pizza/pizzaCheese.ppm", 1280, 720);
	Texture tPizzaSpecial = Texture("./objects/pizza/pizzaSpecial.ppm", 1280, 720);
	Texture tPizzaCrust = Texture("./objects/pizza/pizzaCrust.ppm", 1280, 720);

	//Topping
	poly = new Polyhedron();
	poly->loadObj("./objects/pizza/pizzaTop.obj", .2);
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
	poly->loadObj("./objects/pizza/pizzaCrust.obj", .2);
	poly->textureInit(tPizzaCrust);
	polyhedrons.push_back(poly);
	//Bottom
	poly = new Polyhedron();
	poly->loadObj("./objects/pizza/pizzaBottom.obj", .2);
	poly->textureInit(tPizzaCrust);
	polyhedrons.push_back(poly);

	translate(tX,tY,tZ);
}

void Object::makeHouse(GLfloat tX, GLfloat tY, GLfloat tZ) {
	polyhedrons.clear();
	//Load textures
	Texture tPizzaPep = Texture("./objects/pizza/pizzaPep.ppm", 1280, 720);
	Texture tPizzaCheese = Texture("./objects/pizza/pizzaCheese.ppm", 1280, 720);
	Texture tPizzaSpecial = Texture("./objects/pizza/pizzaSpecial.ppm", 1280, 720);
	Texture tPizzaCrust = Texture("./objects/pizza/pizzaCrust.ppm", 1280, 720);
	Texture redBoards = Texture("./objects/house/red_boards.ppm", 1280, 720);
	Texture bricks = Texture("./objects/house/bricks.ppm", 1280, 720);
	Texture chimney = Texture("./objects/house/chimney.ppm", 1280, 720);
	Texture white = Texture("./objects/house/white_wall.ppm", 1280, 720);
	Texture drywall = Texture("./objects/house/dry_wall.ppm", 1280, 720);
	Texture door = Texture("./objects/house/door.ppm", 1280, 720);
	Texture window = Texture("./objects/house/window.ppm", 1280, 720);

	//House Base
	poly = new Polyhedron();
	poly->loadObj("./objects/house/house_base.obj", .2);
	poly->textureInit(drywall);
	polyhedrons.push_back(poly);
	//Roof
	poly = new Polyhedron();
	poly->loadObj("./objects/house/roof.obj", .2);
	poly->textureInit(redBoards);
	polyhedrons.push_back(poly);
	//Steps
	poly = new Polyhedron();
	poly->loadObj("./objects/house/steps.obj", .2);
	poly->init();
	polyhedrons.push_back(poly);
	//Steps
	poly = new Polyhedron();
	poly->loadObj("./objects/house/chimney.obj", .2);
	poly->textureInit(chimney);
	polyhedrons.push_back(poly);
	//door_handle_base
	poly = new Polyhedron();
	poly->loadObj("./objects/house/door_handle_base.obj", .2);
	poly->textureInit(tPizzaCheese);
	polyhedrons.push_back(poly);
	//door_base
	poly = new Polyhedron();
	poly->loadObj("./objects/house/door_base.obj", .2);
	poly->textureInit(door);
	polyhedrons.push_back(poly);
	//door_handle
	poly = new Polyhedron();
	poly->loadObj("./objects/house/door_handle.obj", .2);
	poly->textureInit(tPizzaCheese);
	polyhedrons.push_back(poly);
	//window 1
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window1.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);
	//window 2
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window2.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);
	//window 3
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window3.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);
	//window 4
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window4.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);
	//window 5
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window5.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);
	//window 6
	poly = new Polyhedron();
	poly->loadObj("./objects/house/window6.obj", .2);
	poly->textureInit(window);
	polyhedrons.push_back(poly);

	translate(tX,tY,tZ);
}

int Object::random(int p) {
	int r = int(((float) rand() / RAND_MAX *p))+1;
	cout << r << endl;
	return r;
}

void Object::updateTexture(Texture t) {
	//abstract
}
