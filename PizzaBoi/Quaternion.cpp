#include "Quaternion.h"

Quaternion::Quaternion() {};

double Quaternion::length() {
	return sqrt(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::normalize() {
	Quaternion Q;
	double L = length();
	Q.x = x / L;
	Q.y = y / L;
	Q.z = z / L;
	Q.w = w / L;
	return Q;
}

Quaternion Quaternion :: conjugate() {
	Quaternion Q;
	Q.x = -x;
	Q.y = -y;
	Q.z = -z;
	return Q;
}

Quaternion Quaternion::mult(Quaternion B) {
	Quaternion Q;

	Q.x = w*B.x + x*B.w + y*B.z - z*B.y;
	Q.y = w*B.y - x*B.z + y*B.w + z*B.x;
	Q.z = w*B.z + x*B.y - y*B.x + z*B.w;
	Q.w = w*B.w - x*B.x - y*B.y - z*B.z;
	return Q;
}