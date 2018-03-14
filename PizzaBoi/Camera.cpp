#include "Camera.h"

Camera::Camera() {

}

Camera::Camera(vec4 e, vec4 a, vec4 u) { //eye (orgin), at, up

	projection = Perspective(65,1.7777,1,100);
	ortho = false;

	xA = 0;
	yA = 0;
	zA = 0;

	eye = e;
	at  = a;
	up  = u;

	setCameraMatrix(LookAt(eye, at, up));
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