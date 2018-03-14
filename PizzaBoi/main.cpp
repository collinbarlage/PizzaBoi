#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Polyhedron.h"  //blue box object!
#include "Sphere.h"  //a ball!?
#include <cstdlib>
#include <ctime>

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

//helper functions
void toggleKey(unsigned char key, bool toggle);
void toggleSpecialInput(int key, bool toggle);
void animateKeys();

struct Key{
	char key;
	bool pressed;
};

struct SpecialInput{
	int key;
	bool pressed;
};


//Objects
Polyhedron* mbox;
Polyhedron* object;
Sphere* sphere;
Camera cam1 = Camera(vec4(0,2,-2,1), vec4(0,0,0,1), vec4(0,1,0,1));
Camera cam2 = Camera(vec4(0,-10,0,1), vec4(1,-10,0,1), vec4(0,1,0,1));
Camera *cam = &cam1;
vector<Drawable*>drawables;

//Lights
vector<Light*> lights;
Light sun = Light(vec4(2,10,0,1),vec4(.9,.7,.5,1),vec4(1,1,1,1),vec4(.3,1,1,1));
Light flashlight = Light(vec4(0,2,-2,1),vec4(.2,.1,.1,1),vec4(.3,.1,0,1),vec4(.5,.5,.1,1));
float orbitTime = 0;


//Helpers
bool camSelect = false;
bool stoneSelect = false;
mat4 getCameraMatrix();
vec4 getCameraEye();
GLuint windowID=0;

//Put any keys here that you want to be animated when held down
enum E_Keys {z=0, Z, xKey, X, c, C, KEYS_SIZE}; //make sure KEYS_SIZE is always last element in enum
Key keys[] = {{'z', false},
              {'Z', false},
              {'x', false},
              {'X', false},
              {'c', false},
              {'C', false}};

//Put any special inputs here that you want to be animated when held down
enum E_SInputs {Up=0, Down, Left, Right, SINPUTS_SIZE}; //make sure SINPUTS_SIZE is always last element in enum
SpecialInput sInputs[] = {{GLUT_KEY_UP, false},
						  {GLUT_KEY_DOWN, false},
						  {GLUT_KEY_LEFT, false},
						  {GLUT_KEY_RIGHT, false}};


//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand(time(NULL));
	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

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

	init(); //initalize objects

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

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

//scene
	//lights
	lights.push_back(&sun);
	flashlight.on = 0; //initilize flashlight to be off
	lights.push_back(&flashlight);

	//sphere
	sphere = new Sphere(64,vec4(5.0, 0.794, 0.886,1),vec4(0.1, 0.694, 0.986,1),vec4(0.0, 0.1, 0.2,1));
	sphere->setModelMatrix(Translate(1,-.4,1));
	sphere->init();
	//drawables.push_back(sphere);

	//cube
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(2,-10,0)*Scale(1,1.8,3.2));
	mbox->textureInit("pizzaBoiMainMenu.ppm");
	drawables.push_back(mbox);

	//floor plane
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	//mbox->setModelMatrix(Translate(-1,-.4,-1));
	mbox->setModelMatrix(Translate(0,-1.5,0)*Scale(15.0,0.1,15.0));
	mbox->textureInit("grass.ppm");
	drawables.push_back(mbox);

	//object
	//object = new Polyhedron();
	//object->loadObj("totem.obj", 1);
	//object->setModelMatrix(Translate(-2, -.5, 1));
	//object->init();
	//object->setModelMatrix(Translate(0, 0, 0));
	//drawables.push_back(object);


	//skybox
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(0,0,0)*Scale(50.0,50.0,50.0));
	mbox->textureInit("sky.ppm");
	drawables.push_back(mbox);
	
	//orbit sun
	timerCallback(0);
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
	float xp, yp;

	if(state == 0) {
		//cout << "CLICKED at " << x << ", " << y << endl;
		xp = (2*float(x)/512)-1;
		yp = 1-(2*float(y)/512);
		//cout << "xp = " << xp << "yp = " << yp << endl;
		vec4 pFront = vec4(xp, yp, -1, 1);
		vec4 pCam = inverse(getCameraMatrix())*pFront;
		vec4 pWorld = inverse(getCameraMatrix())*pCam;
		vec4 rayWorld = pWorld - getCameraEye();

		drawables[1]->pick(rayWorld, getCameraEye());
		// display();
	}	
}

void keyboard(unsigned char key, int x, int y)
{
	//put keys here that aren't meant to be held down / animated
	if(key == 'q' || key == 'Q') close();
	if (key == 'p' || key == 'P') cam->toggleProj();
	if (key == ' ') cam = &cam2;
	// if (key == ' ') camToggle = !camToggle;

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
	//continue animating
	glutTimerFunc(10, timerCallback, 0);
	glutPostRedisplay();
}

//Holds all keys and their animations. Meant for the timerCallback function
void animateKeys() {
	if (keys[z].pressed)  			cam->rotate(0, 0, -1);
	if (keys[Z].pressed)  			cam->rotate(0, 0, 1);
	if (keys[c].pressed)  			cam->rotate(0, -1, 0);
	if (keys[C].pressed)  			cam->rotate(0, 1, 0);
	if (keys[xKey].pressed)  		cam->rotate(-1, 0, 0);
	if (keys[X].pressed)  			cam->rotate(1, 0, 0);

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