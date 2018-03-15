#ifndef __QUATERNION_H__
#define __QUATERNION_H__
#include "Angel.h"

class Quaternion {
public: 
	Quaternion();
	double length();
	Quaternion normalize();
	Quaternion conjugate();
	Quaternion mult(Quaternion B);

	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;

	
};

#endif
