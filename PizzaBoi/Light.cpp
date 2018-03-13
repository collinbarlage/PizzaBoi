#include "Light.h"



void Light::move(GLfloat x, GLfloat y, GLfloat z) {
	vec4 pos = position;
	position = vec4(pos.x+x, pos.y+y, pos.z+z, 1);

}



