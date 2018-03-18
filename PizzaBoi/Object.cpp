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
	Texture tPizzaPep = Texture("./objects/pizzaPep.ppm", 1280, 720);
	Texture tPizzaCheese = Texture("./objects/pizzaCheese.ppm", 1280, 720);
	Texture tPizzaSpecial = Texture("./objects/pizzaSpecial.ppm", 1280, 720);
	Texture tPizzaCrust = Texture("./objects/pizzaCrust.ppm", 1280, 720);

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

	translate(tX,tY,tZ);
}

void Object::makeHouse(GLfloat tX, GLfloat tY, GLfloat tZ) {
	//TODO: model a haus!

	//Load textures

	//...

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
