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
void arrows(int key, int x, int y);
void click(int button, int state, int x, int y);
void resize(int width, int height);
void close(void);
void timerCallback(int value);


//Objects
Polyhedron* mbox;
Sphere* sphere;
Camera cam1 = Camera(vec4(0,2,-2,1), vec4(0,1,0,1));
Camera cam2 = Camera(vec4(0,10,0,1), vec4(0,0,-1,1));
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


//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand(time(NULL));
	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);

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
	glutSpecialFunc(arrows);
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
	drawables.push_back(sphere);

	//cube
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(-1,-.4,-1));
	mbox->init("bloc.ppm");
	drawables.push_back(mbox);

	//floor plane
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	//mbox->setModelMatrix(Translate(-1,-.4,-1));
	mbox->setModelMatrix(Translate(0,-1.5,0)*Scale(15.0,0.1,15.0));
	mbox->init("grass.ppm");
	drawables.push_back(mbox);

	//skybox
	mbox = new Polyhedron();
	mbox->loadSmf("cube");
	mbox->setModelMatrix(Translate(0,0,0)*Scale(50.0,50.0,50.0));
	mbox->init("sky.ppm");
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
		drawables[i]->draw(cam1, lights);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:  // Escape key
	case 'q': case 'Q':
		close();
		break;
	case 'z': 
		cam1.rotate(0,0,-1);
		break;
	case 'Z': 
		cam1.rotate(0,0,1);
		break;
	case 'c': 
		cam1.rotate(0,-1,0);
		break;
	case 'C': 
		cam1.rotate(0,1,0);
		break;
	case 'x': 
		cam1.rotate(-1,0,0);
		break;
	case 'X': 
		cam1.rotate(1,0,0);
		break;
	case ' ': 
		//Flashlight toggle
		flashlight.on = !flashlight.isOn(); //toggle
		break;
	case 'p': case 'P':
		cam1.toggleProj();
		break;
	case 't': case 'T':
		if(stoneSelect) {
			drawables[1]->makeTexture("bloc.ppm");
		} else {
			drawables[1]->makeTexture("stone.ppm");
		}
		stoneSelect = !stoneSelect; //toggle
		break;
	}
	display();
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
		display();
	}	
}

void arrows(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cam1.move(-.1,0,0);
		flashlight.move(.1,0,0);
		break;

	case GLUT_KEY_RIGHT:
		cam1.move(.1,0,0);
		flashlight.move(-.1,0,0);
		break;

	case GLUT_KEY_UP:
		cam1.move(0,0,-.1);
		flashlight.move(0,0,.1);
		break;

	case GLUT_KEY_DOWN:
		cam1.move(0,0,.1);
		flashlight.move(0,0,-.1);
		break;
	}
	glutPostRedisplay();
	display();
}

//----------------------------------------------------------------------------
//Timer  callback
void timerCallback(int value) {
	vec4 pos = sun.position;
	//Rotate sun
	orbitTime += .002;
	if(orbitTime >= 3.1415926535) 
		orbitTime = -3.1415926535;

	sun.position = vec4(cos(orbitTime)*10, sin(orbitTime)*10, pos.z, 1);
	//continue animating
	glutTimerFunc(10, timerCallback, 0);
	glutPostRedisplay();
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);

    exit(EXIT_SUCCESS);
}

mat4 getCameraMatrix(){
	if(camSelect) {
		return cam2.cameraMatrix;
	} else {
		return cam1.cameraMatrix;
	}
}

vec4 getCameraEye(){
	if(camSelect) {
		return cam2.eye;
	} else {
		return cam1.eye;
	}
}