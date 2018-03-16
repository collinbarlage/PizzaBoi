#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

class Sphere: public Drawable{


public:
	Sphere(unsigned int i);
	Sphere(unsigned int i, vec4 diff, vec4 spec, vec4 amb);
	~Sphere();

	void init();
	void addVert(vec4 v);
	void addVert(vec4 v, vec4 c);
	void draw(Camera, vector<Light*>);
	void pick(vec4 probe, vec4 eye);
	void makeWireframe();
	void updateTexture(Texture t);


private:
	void buildSphere();
	void makeTriangle(vec4 a, vec4 b, vec4 c);
	void divideTriangle(vec4 a, vec4 b, vec4 c, int i);
	bool intersects(vec4 ray, vec4 eye, vec4 a, vec4 b, vec4 c);
	vec4 norm(vec4 p);
	GLuint vPosition;
	GLuint vColor;
	GLuint vNormal;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	GLuint npos;
	GLuint diffuse_loc;
	GLuint spec_loc;
	GLuint ambient_loc;
	GLuint alpha_loc;
	vec4 randomColor();
	GLfloat randomFloat();

	unsigned int index;
	unsigned int numVerts;
	bool wireframe;
	vec4 spec;
	vec4 diff;
	vec4 ambi;

	vector<vec4> points; //6 faces, 2 triangles/face, 3 vertices per triangle
	vector<vec4> colors;
	vector<vec3> normals;
};
#endif