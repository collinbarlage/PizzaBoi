#ifndef __Texture_H__
#define __Texture_H__

#include "Angel.h"
#include <string>

using namespace std;

class Texture {
public:
	Texture();
	Texture(char* fileName, int x, int y);
	~Texture();

	unsigned char* getImage();
	void setImage(unsigned char* i);

	char* name;
	bool isLoaded;
	int width;
	int height;
private:
	unsigned char* ppmRead(char* filename, int* width, int* height);
	unsigned char* image;

	
};

#endif
