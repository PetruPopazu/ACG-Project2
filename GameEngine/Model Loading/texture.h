#pragma once
#include <glew.h>
#include <glfw3.h>

GLuint loadBMP(const char * imagepath);
unsigned char* loadRawBMP(const char* imagepath, unsigned int& width, unsigned int& height);