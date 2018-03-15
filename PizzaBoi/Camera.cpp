#include "Camera.h"
#define PI  3.14159265358979323846

Camera::Camera() {

}

Camera::Camera(vec4 e, vec4 u) {

	projection = Perspective(65,1,1,100);
	ortho = false;


	xA = 0;
	yA = 0;
	zA = 0;

	eye = e;
	at  = vec4(0,0,0,1);
	up  = u;

	setCameraMatrix(LookAt(eye, at, up));
}

void Camera::fpsRotate(GLfloat x, GLfloat y, GLfloat z){

	xA += -x; yA += -y; zA += -z;

	vec4 inverseEye = eye * -1;

	setCameraMatrix(cameraMatrix*Translate(inverseEye));

	setCameraMatrix(cameraMatrix*RotateX(-x)*RotateY(-y)*RotateZ(-z));

	setCameraMatrix(cameraMatrix*Translate(eye));
}

void Camera::fpsMove(GLfloat x, GLfloat y, GLfloat z, bool forward /* foward means it goes foward/backward or left/right*/) {
	
	GLfloat rotatedZ = 0;
	GLfloat rotatedX = 0;
	if (forward) {
		rotatedZ = cos((yA / 180.0)*PI) * z;
		rotatedX = sin((yA / 180.0)*PI) * x;
	} else {
		rotatedZ = sin((yA / 180.0)*PI) * z;
		rotatedX = cos((yA / 180.0)*PI) * x;
	}


	at += vec4(rotatedX, y, rotatedZ, 0);
	eye += vec4(rotatedX, y, rotatedZ, 0);

	setCameraMatrix(cameraMatrix*Translate(rotatedX, y, rotatedZ));
}


void Camera::move(GLfloat x, GLfloat y, GLfloat z) {
	at += vec4(x,y,z,0);
	eye += vec4(x,y,z,0);

	setCameraMatrix(cameraMatrix*Translate(x,y,z));
}

void Camera::rotate(GLfloat x, GLfloat y, GLfloat z) {
	xA += x; yA += y; zA += z;
	setCameraMatrix(cameraMatrix*RotateX(x)*RotateY(y)*RotateZ(z));
}

void Camera::toggleProj() {
	if(ortho){
		projection = Perspective(65,1,1,100);
		ortho = false;
	} else {
		projection = Ortho(-1,1,-1,1,-10,10);
		ortho = true;
	}

	calcProj();
}

void Camera::calcProj() {
	setCameraMatrix(LookAt(eye, at, up));
	setCameraMatrix(cameraMatrix*RotateX(xA)*RotateY(yA)*RotateZ(zA));

}