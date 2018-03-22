//-----------------------------//
//        ~ PIZZA BOI ~	       //
//  A WONDER TWINZ PRODUCTION  //
//-----------------------------//
//	~ � 2018 Wonder Twinz	   //
//	~ MATTHEW WAGAR			  //
//	~ COLLIN BARLAGE		 //
//--------------------------//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Polyhedron.h"  //blue box object!
#include "Object.h"  //a pizza
#include "Texture.h"  //a pic
#include "Particle.h"  //a pic
#include <cstdlib>
#include <ctime>

//Screen size
int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void keyboardup(unsigned char, int, int);
void specialInput(int key, int x, int y);
void specialInputUp(int key, int x, int y);
void click(int button, int state, int x, int y);
void resize(int width, int height);
void close(void);
void timerCallback(int value);
void setViewByMouse(int x, int y);

struct Key{
	char key;
	bool pressed;
};

struct SpecialInput{
	int key;
	bool pressed;
};

//game functions
void startGame(void);
void die(void);
void firePizza(vec4 at);
void spawnMob(int t, vec4 at);

//helper functions
void toggleKey(unsigned char key, bool toggle);
void toggleSpecialInput(int key, bool toggle);
void animateKeys();
void detectCollisions(int i);
void detectMobCollisions(int i);

//Objects
Polyhedron* mbox;
Object* object = new Object(); //Loads all textures for objects
Object* house = new Object(); //Loads all textures for objects
Camera cam1 = Camera(vec4(0,0,0.5,1), vec4(0,0,0,1), vec4(0, 1, 0, 1));
Camera cam2 = Camera(vec4(0,-10,0,1), vec4(1,-10,0,1), vec4(0,1,0,1));
Camera *cam = &cam2;
vector<Drawable*>drawables;

//Particles
Particle* particle;
vector<Particle*> particles;

//Projectiles
int ammoIndex = 0;
vector<Object*> ammo;
vector<Object*> houses;

//Mob
int numMob;
int mobIndex = 0;
vector<Object*> mob;

//Lights
vector<Light*> lights;
Light sun = Light(vec4(2,10,0,1),vec4(.7,.7,.5,1),vec4(1,1,1,1),vec4(.3,1,1,1));
Light flashlight = Light(vec4(0,2,-2,1),vec4(.2,.1,.1,1),vec4(.3,.1,0,1),vec4(.5,.5,.1,1));
float orbitTime = 0;

//Textures (just global textures. Other textures loaded in Object() constructor)
Texture tMainMenu = Texture("pizzaBoiMainMenu.ppm", 1280, 720);
Texture tLoad = Texture("pizzaBoiLoad.ppm", 1280, 720);
Texture tDeath = Texture("pizzaBoiDeath.ppm", 1280, 720);
Texture tGrass = Texture("objects/grass.ppm", 1280, 720);
Texture tSky = Texture("objects/skyRect.ppm", 1280, 720);
Texture tBrick = Texture("objects/brick.ppm", 1280, 720);
Texture tStone = Texture("objects/cobblestone.ppm", 1280, 720);

Texture tSnow = Texture("objects/arctic.ppm", 1280, 720);
Texture tTiger = Texture("objects/lion.ppm", 1280, 720);
Texture tCheeta = Texture("objects/cheeta.ppm", 1280, 720);

//Helpers
int t = 0; //time
bool isAtMainMenu = true;
bool stoneSelect = false;
mat4 getCameraMatrix();
vec4 getCameraEye();
GLuint windowID=0;
int numHouses;

//Put any keys here that you want to be animated when held down
enum E_Keys {z=0, Z, xKey, X, c, C, a, s, d, w, KEYS_SIZE}; //make sure KEYS_SIZE is always last element in enum
Key keys[] = {{'z', false},
              {'Z', false},
              {'x', false},
              {'X', false},
			  {'c', false},
			  {'C', false},
		      {'a', false},
			  {'s', false},
			  {'d', false},
              {'w', false}};

//Put any special inputs here that you want to be animated when held down
enum E_SInputs {Up=0, Down, Left, Right, SINPUTS_SIZE}; //make sure SINPUTS_SIZE is always last element in enum
SpecialInput sInputs[] = {{GLUT_KEY_UP, false},
						  {GLUT_KEY_DOWN, false},
						  {GLUT_KEY_LEFT, false},
						  {GLUT_KEY_RIGHT, false}};

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	cout <<"* PIZAA - BOI *\n  *** A WONDER TWINZ PRODUCTION ***" << endl;

	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);

	//Screen title
	windowID = glutCreateWindow("PIZZA BOI");

	//print out info about our system
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//initialize glew 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//initalize objects
	init(); 

	//set up the callback functions
	glutDisplayFunc(display);
	glutWMCloseFunc(close);
	glutKeyboardFunc(keyboard);  //What to do if a keyboard event is detected
	glutKeyboardUpFunc(keyboardup);  //What to do if a keyboard event is detected
	glutSpecialFunc(specialInput);
	glutSpecialUpFunc(specialInputUp);
	glutMouseFunc(click);

	//start the main event listening loop
	glutMainLoop();
	return 0;
}

//Scene - Load all geometry off screen for spawing
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//lights
	lights.push_back(&sun);
	flashlight.on = 0; //initilize flashlight to be off
	lights.push_back(&flashlight);
	
	//main menu
	mbox = new Polyhedron();
	mbox->loadObj("mainMenu.obj", 1);
	mbox->setModelMatrix(Translate(2,-10,0)*Scale(1.5,1.5,1.5)*RotateY(90));
	mbox->textureInit(tMainMenu);
	drawables.push_back(mbox);

	//Grass
	mbox = new Polyhedron();
	mbox->loadObj("objects/grass.obj", 1);
	mbox->setModelMatrix(Translate(0,-1,0));
	mbox->textureInit(tGrass);
	drawables.push_back(mbox);

	//Sidewalk
	mbox = new Polyhedron();
	mbox->loadObj("objects/sidewalk.obj", 1);
	mbox->setModelMatrix(Translate(0,-1,0));
	mbox->textureInit(tBrick);
	drawables.push_back(mbox);

	//Street
	mbox = new Polyhedron();
	mbox->loadObj("objects/street.obj", 1);
	mbox->setModelMatrix(Translate(0,-1,0));
	mbox->textureInit(tStone);
	drawables.push_back(mbox);

	//Skybox
	mbox = new Polyhedron();
	mbox->loadObj("objects/skyRect.obj", 1);
	mbox->setModelMatrix(Scale(.2,.2,.2));
	mbox->textureInit(tSky);
	drawables.push_back(mbox);


	numHouses = 6; //make even number plz
	//Make houses left
	for(int i=0; i<numHouses/2; i++) {
		object = new Object();
		object->makeHouse(19+(30*i),.5,7);
		object->rotate(180);
		drawables.push_back(object);
		houses.push_back(object);
	}
	//Make houses right
	for(int i=0; i<numHouses/2; i++) {
		object = new Object();
		object->makeHouse(4+(30*i),.5,-7);
		drawables.push_back(object);
		houses.push_back(object);
	}

	//Spawn Pizzas, Mama Mia
	cout << "pizzas...";
	srand(time(0));
	for(int i=1; i<=10; i++) {
		object = new Object();
		object->makePizza(0,-5,0);
		drawables.push_back(object);
		ammo.push_back(object);
	}

	//skullz
	numMob = 40;
	cout << "skullz...";
	for(int i=0; i<numMob; i++) {
		object = new Object();
		object->makeSkull(0,-20,0);
		drawables.push_back(object);
		mob.push_back(object);
	}
	cout << "done" << endl;

	//orbit sun
	timerCallback(0);
}

//Game Start
void startGame() {
	//Show loading screen
	drawables[0]->updateTexture(tLoad);
	display();
	
	//Load Game
	isAtMainMenu = false;
	cam = &cam1;
	drawables[0]->updateTexture(tDeath);

	//set up mouse
	glutPassiveMotionFunc(setViewByMouse);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void firePizza(vec4 at) {
	//adjust speed here
	int speed = 3; //1 is fast af, 10 is slowww

	//adjust for camera starting at 0,0,.5
	at.z += .5;
	//vector of angle shot
	vec3 angle = vec3(cos(DegreesToRadians*(cam1.yA+90)), 0, sin(DegreesToRadians*(cam1.yA+90)));
	//trigger animation
	ammo[ammoIndex]->stopAnimation(); //stop previous animation if any
	ammo[ammoIndex]->spawn(-at.x, -0.5,-at.z);
	ammo[ammoIndex]->setAnimation(vec3(angle.x/-speed, 0, angle.z/-speed), 1);
	//incriment ammo index
	ammoIndex++;
	if(ammoIndex >= 10) //only allow 10 pizzas on screen at once
		ammoIndex = 0;
}


void spawnMob(int t, vec4 at) {
	float speed = .1; //1 is fast af, .1 is slowww
	mob[mobIndex]->stopAnimation();
	mob[mobIndex]->spawn(-at.x+50, -0.5,40*((float)rand()/RAND_MAX)-20);
	mob[mobIndex]->setAnimation(vec3(-speed, 0, 0), 1);
	mobIndex++;
	if(mobIndex >= numMob) //only allow 10 pizzas on screen at once
		mobIndex = 0;
}

void die() { //call when player dies
	isAtMainMenu = true;
	//undo fps camera
	glutSetCursor(GLUT_CURSOR_INHERIT);
	//reset cameras
	cam1 = Camera(vec4(0,0,0.5,1), vec4(0,0,0,1), vec4(0, 1, 0, 1));
	cam2 = Camera(vec4(0,-10,0,1), vec4(1,-10,0,1), vec4(0,1,0,1));
	cam = &cam2;

	//Reset houses
	int h = 0;
	for(int i=0; i<numHouses/2; i++) {
		houses[h]->spawn(19+(30*i),.5,7);
		h++;
	}
	//Make houses right
	for(int i=0; i<numHouses/2; i++) {
		houses[h]->spawn(4+(30*i),.5,-7);
		h++;
	}

}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//cout << lights[0]->position << endl;
	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(Camera(*cam), lights);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}

void click(int button, int state, int x, int y) {
	srand(time(NULL));

	if(isAtMainMenu && state == 1 && x >440 && x < 889 && y > 443 && y < 690) {
		//START GAME
		startGame();
		display();
	} else if (state == 1){
		//FIRE PIZZA
		firePizza(cam1.at);
	}
}

void keyboard(unsigned char key, int x, int y)	{
	//put keys here that aren't meant to be held down / animated
	if(key == 'q' || key == 'Q') close();
	if (key == 'p' || key == 'P') cam->toggleProj();
	if (key == ' ') {
		die();
	}
	toggleKey(key, true);
}

void keyboardup(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') close();
	toggleKey(key, false);
}

//Toggles key pressed on/off 
void toggleKey(unsigned char key, bool toggle){
	for (unsigned int i = 0; i < KEYS_SIZE; i++) {
		if (key == keys[i].key) {
			keys[i].pressed = toggle; 
			break; 
		}
	}
}

void specialInput(int key, int x, int y) {
	toggleSpecialInput(key, true);
}

void specialInputUp(int key, int x, int y) {
	toggleSpecialInput(key, false);
}

//Toggles special key pressed on/off 
void toggleSpecialInput(int key, bool toggle){
	for (unsigned int i = 0; i < SINPUTS_SIZE; i++) {
		if (key == sInputs[i].key) {
			sInputs[i].pressed = toggle; 
			break; 
		}
	}
};


//----------------------------------------------------------------------------
//Timer  callback
void timerCallback(int value) {
	vec4 pos = sun.position;

	animateKeys();

	//Rotate sun
	orbitTime += .002;
	if(orbitTime >= 3.1415926535) 
		orbitTime = -3.1415926535;
	sun.position = vec4(cos(orbitTime)*10, sin(orbitTime)*10, pos.z, 1);

	//Spawn mob
	if(!isAtMainMenu && t%20 == 0) { //spawn rate
		spawnMob(t, cam1.at);
	}

	//Move projectiles
	for(int i=0; i<ammo.size(); i++) {
		ammo[i]->animate();
		detectCollisions(i);
	}

	//Move mob
	for(int i=0; i<numMob; i++) {
		mob[i]->animate();
		detectMobCollisions(i);
	}

	//move particles
	for(int i=0; i<particles.size(); i++) {
		particles[i]->idle();
		particles[i]->draw(Camera(*cam), lights);
	}



	t++; //incriment time
	//continue animating...
	glutTimerFunc(10, timerCallback, 0);
	glutPostRedisplay();
}

void detectMobCollisions(int i) {
	vec3 p1 = mob[i]->getLocation();
	vec3 p2 = vec3(-cam1.at.x, cam1.at.y,-cam1.at.z);

	float distance = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2) + pow((p2.z - p1.z), 2));
	if (distance < 1) { //THIS IS WHERE IT FINALLY COLLIDES
		die();
	}

}


void detectCollisions(int i) {
	//pizza->skull
	for (int j = 0; j < numMob; j++) {
		vec3 p1 = ammo[i]->getLocation();
		vec3 p2 = mob[j]->getLocation();

		float distance = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2) + pow((p2.z - p1.z), 2));

		if (distance < 1.5) { //if its in the hitbox range

				//make particlez
				particle = new Particle(p1.x, p1.y, p1.z,true);
				particle->init();
				particles.push_back(particle);
				drawables.push_back(particle);
				//respawn geometry
				mob[j]->spawn(0,-20,0);
				ammo[i]->spawn(0, -5, 0);
				ammo[i]->stopAnimation(); //stop previous animation if any
		}
	}
	//pizza->houses
	for (int j = 0; j < houses.size(); j++) {
		vec3 p1 = ammo[i]->getLocation();
		vec3 p2 = houses[j]->getLocation();

		vec3 houseSquare[] = { vec3(p2.x + 2, p2.y, p2.z + 1.5), vec3(p2.x - 2, p2.y, p2.z + 1.5), vec3(p2.x - 2, p2.y, p2.z - 1.5), vec3(p2.x + 2, p2.y, p2.z - 1.5) };
		float distance = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2) + pow((p2.z - p1.z), 2));

		if (distance < 4) { //if its in the hitbox range
			for (int k = 0; k < 4; k++) {
				distance = sqrt(pow((houseSquare[k].x - p1.x), 2) + pow((houseSquare[k].y - p1.y), 2) + pow((houseSquare[k].z - p1.z), 2));

				if (distance < 2) { //THIS IS WHERE IT FINALLY COLLIDES
					//make particlez
					particle = new Particle(p1.x, p1.y, p1.z,false);
					particle->init();
					particles.push_back(particle);
					drawables.push_back(particle);
					//respawn geometry
					houses[j]->spawn(0,-5,0);
					ammo[i]->spawn(0, -5, 0);
					ammo[i]->stopAnimation(); //stop previous animation if any
				}
			}
		}
	}
}

//Holds all keys and their animations. Meant for the timerCallback function
void animateKeys() {
	if(!isAtMainMenu) {
		if (keys[z].pressed)  			cam->rotate(0, 0, -1);
		if (keys[Z].pressed)  			cam->rotate(0, 0, 1);
		if (keys[c].pressed)  			cam->rotate(0, -1, 0);
		if (keys[C].pressed)  			cam->rotate(0, 1, 0);
		if (keys[xKey].pressed)  		cam->rotate(-1, 0, 0);
		if (keys[X].pressed)  			cam->rotate(1, 0, 0);
	
		if (keys[w].pressed)  			cam->fpsMove(-0.05, 0, 0.05, true);
		if (keys[s].pressed)  			cam->fpsMove(0.05, 0, -0.05, true);
		if (keys[a].pressed)  			cam->fpsMove(0.05, 0, 0.05, false);
		if (keys[d].pressed)  			cam->fpsMove(-0.05, 0, -0.05, false);


		if (sInputs[Up].pressed) {
			cam->move(0, 0, -.05);
			flashlight.move(0, 0, .05);
		}
		if (sInputs[Down].pressed) {
			cam->move(0, 0, .05);
			flashlight.move(0, 0, -.05);
		}
		if (sInputs[Left].pressed) {
			cam->move(-.05, 0, 0);
			flashlight.move(.05, 0, 0);
		}
		if (sInputs[Right].pressed) {
			cam->move(.05, 0, 0);
			flashlight.move(-.05, 0, 0);
		}
	}
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);
    exit(EXIT_SUCCESS);
}

mat4 getCameraMatrix(){
	return cam->cameraMatrix;
}

vec4 getCameraEye(){
	return cam->eye;
}

//mouse over event
void setViewByMouse(int x, int y)
{
	if(!isAtMainMenu) {
		GLfloat MouseSensitivity = 10;
		GLfloat CurrentRotationX = cam->xA;

		// the coordinates of our mouse coordinates
		int MouseX = x;
		int MouseY = y;

		// the middle of the screen in the x direction
		int MiddleX = SCREENWIDTH / 2;

		// the middle of the screen in the y direction
		int MiddleY = SCREENHEIGHT / 2;

		// vector that describes mouseposition - center
		vec3 MouseDirection = vec3(0.0, 0.0, 0.0);

		static GLfloat CurrentRotationAboutX = 0.0;

		// The maximum angle we can look up or down, in radians
		double maxAngle = 1;

		// if the mouse hasn't moved, return without doing
		// anything to our view
		if ((MouseX == MiddleX) && (MouseY == MiddleY))
			return;

		glutWarpPointer(MiddleX, MiddleY);
		MouseDirection.x = (MiddleX - MouseX) / MouseSensitivity;
		MouseDirection.y = (MiddleY - MouseY) / MouseSensitivity;

		cam->fpsRotate(0, MouseDirection.x, 0);
	}
}