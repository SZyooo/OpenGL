#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <gl/glew.h>
#include <SOIL2/SOIL2.h>
GLuint loadTexture(const char*);
std::string readShaderSource(const char*);
void printShaderLog(GLuint);
void printProgramLog(int);
bool checkOpenGLError();
GLuint createShaderProgram(const char*, const char*);
