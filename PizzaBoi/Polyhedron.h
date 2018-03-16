#ifndef __Polyhedron_H__
#define __Polyhedron_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class Polyhedron: public Drawable{


public:
	Polyhedron();
	Polyhedron(vec4 diff, vec4 spec, vec4 ambi);
	~Polyhedron();

	Polyhedron(vector<vec4> verts);

	void init();
	void textureInit(Texture t);
	void updateTexture(Texture t);
	void addVert(vec4 v);
	void addVert(vec4 v, vec4 c);
	void draw(Camera, vector<Light*>);
	void makeWireframe();
	void loadSmf(string filename);
	void loadObj(string filename, float scale);

private:
	void buildPolyhedron();
	void buildCube();
	void calcNormals();
	void makeQuad(GLuint a, GLuint b, GLuint c, GLuint d);
	GLuint vPosition;
	GLuint vColor;
	GLuint vNormal;
	GLuint vTex;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	GLuint texLoc;
	GLuint texture;
	GLuint npos;
	GLuint diffuse_loc;
	GLuint spec_loc;
	GLuint ambient_loc;
	GLuint alpha_loc;
	vec4 randomColor();
	GLfloat randomFloat();

	unsigned int index;
	bool wireframe;
	vec4 spec;
	vec4 diff;
	vec4 ambi;

	vector<vec4> vertices;
	vector<vec3> normalVerts;
	vector<vec2> textureVerts;

	vector<vec4> points; //6 faces, 2 triangles/face, 3 vertices per triangle
	vector<vec4> colors;
	vector<vec3> normals;
	vector<vec2> textureCoords;
};
#endif