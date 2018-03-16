#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Polyhedron.h"  //blue box object!
#include "Object.h"  //a pizza
#include "Sphere.h"  //a ball!?
#include <cstdlib>
#include <ctime>

//Forward declarations
void init(void);
void startGame(void);
void kill(void);
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
Object* object;
Sphere* sphere;
Camera cam1 = Camera(vec4(0,0,0.5,1), vec4(0,0,0,1), vec4(0, 1, 0, 1));
Camera cam2 = Camera(vec4(0,-10,0,1), vec4(1,-10,0,1), vec4(0,1,0,1));
Camera *cam = &cam2;
vector<Drawable*>drawables;

//Lights
vector<Light*> lights;
Light sun = Light(vec4(2,10,0,1),vec4(.9,.7,.5,1),vec4(1,1,1,1),vec4(.3,1,1,1));
Light flashlight = Light(vec4(0,2,-2,1),vec4(.2,.1,.1,1),vec4(.3,.1,0,1),vec4(.5,.5,.1,1));
float orbitTime = 0;


//Helpers
bool isAtMainMenu = true;
bool stoneSelect = false;
mat4 getCameraMatrix();
vec4 getCameraEye();
GLuint windowID=0;

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

//Screen size
int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	//srand(time(NULL));
	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);

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
	
	//main menu
	mbox = new Polyhedron();
	mbox->loadObj("mainMenu.obj", 1);
	mbox->setModelMatrix(Translate(2,-10,0)*Scale(1.5,1.5,1.5)*RotateY(90));
	mbox->textureInit("pizzaBoiMainMenu.ppm", 1280, 720);
	drawables.push_back(mbox);

	//sphere
	sphere = new Sphere(64,vec4(5.0, 0.794, 0.886,1),vec4(0.1, 0.694, 0.986,1),vec4(0.0, 0.1, 0.2,1));
	sphere->setModelMatrix(Translate(1,-.4,1));
	sphere->init();
	//drawables.push_back(sphere);

	//floor plane
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(0,-1.5,0)*Scale(15.0,0.1,15.0));
	mbox->textureInit("grass.ppm", 1280, 720);
	drawables.push_back(mbox);

	//totem
	//mbox = new Polyhedron();
	//mbox->loadObj("totem.obj", 1);
	//mbox->setModelMatrix(Translate(-2, -.5, 1));
	//mbox->init();
	//mbox->setModelMatrix(Translate(0, 0, 0));
	//drawables.push_back(mbox);


	//skybox
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(0,0,0)*Scale(90.0,90.0,90.0));
	mbox->textureInit("sky.ppm", 1280, 720);
	drawables.push_back(mbox);
	
	//orbit sun
	timerCallback(0);
}

//Game Start
void startGame() {
	glutPassiveMotionFunc(setViewByMouse);
	glutSetCursor(GLUT_CURSOR_NONE);
	//pizza
		srand(time(0));

	for(float i=1; i<=10; i+=2.25) {
		drawables.push_back(new Object("pizza", Translate(0,-.9,i)));
	}
}

void kill() {
	isAtMainMenu = true;
	//undo fps camera
	glutSetCursor(GLUT_CURSOR_INHERIT);
	//reset cameras
	cam1 = Camera(vec4(0,0,0.5,1), vec4(0,0,0,1), vec4(0, 1, 0, 1));
	cam2 = Camera(vec4(0,-10,0,1), vec4(1,-10,0,1), vec4(0,1,0,1));
	cam = &cam2;


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
		//Show loading screen
		drawables[0]->makeTexture("pizzaBoiLoad.ppm", 1280, 720);
		display();
		//Delete old game
		drawables.clear();
		init();

		//Load Game
		isAtMainMenu = false;
		cam = &cam1;
		drawables[0]->makeTexture("pizzaBoiDeath.ppm", 1280, 720);

		//START GAME
		startGame();
		display();

	}	
}

void keyboard(unsigned char key, int x, int y)
{
	//put keys here that aren't meant to be held down / animated
	if(key == 'q' || key == 'Q') close();
	if (key == 'p' || key == 'P') cam->toggleProj();
	if (key == ' ') {
		kill();
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
	//continue animating
	glutTimerFunc(10, timerCallback, 0);
	glutPostRedisplay();
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