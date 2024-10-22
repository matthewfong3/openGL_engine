#pragma once
#include <GL/glew.h>
#include <fstream>
#include <iostream>
using namespace std;

class ShaderManager
{
private:
	GLuint program;

	GLuint loadShader(const char* file, GLenum shaderType); // private helper-method used by loadShaders() to read and compile a shader file
	// and return the index where it is stored
public:
	GLuint getProgram() const;

	ShaderManager();
	~ShaderManager();

	bool loadShaders(const char* vertexFile, const char* fragmentFile); // public method to load shaders. will set the value of program to 0
	// if it fails; will set value of program to something other than 0 if it succeeds
};

