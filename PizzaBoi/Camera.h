#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {
public:
	Camera();
	Camera(vec4 eye, vec4 at, vec4 up);

	mat4 projection;
	mat4 cameraMatrix;
	vec4 eye;
	vec4 at;
	vec4 up;
	bool ortho;

	void setCameraMatrix(mat4 cm) { cameraMatrix = cm; }
	void move(GLfloat x, GLfloat y, GLfloat z);
	void rotate(GLfloat x, GLfloat y, GLfloat z);

	void toggleProj();

	
private:
	
	GLfloat xA;
	GLfloat yA;
	GLfloat zA;

	void calcProj();
};

#endif
