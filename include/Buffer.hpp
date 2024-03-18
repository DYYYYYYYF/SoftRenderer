#pragma once
#include "Foundation.h"

class Buffer {
public:
	Buffer() {
		Width = 1200;
		Height = 600;
		Pixels = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 1200 * 600);
	}

	Buffer(size_t width, size_t height, int step = 3) {
		Width = width;
		Height = height;
		Pixels = (unsigned char*)malloc(sizeof(unsigned char) * step * width * height);
	}

	virtual ~Buffer() {
		if (Pixels != nullptr) free(Pixels);
	}

	unsigned char* Data() {
		return Pixels;
	}


private:
	unsigned char* Pixels;
	size_t Width, Height;

};