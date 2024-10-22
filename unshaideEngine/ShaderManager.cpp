#include "ShaderManager.h"

ShaderManager::ShaderManager() {
	program = 0;
}

ShaderManager::~ShaderManager() {

}

GLuint ShaderManager::getProgram() const { return program; }

GLuint ShaderManager::loadShader(const char* file, GLenum shaderType) {
	// read the shader code into string
	ifstream input(file, ios::binary);

	if (!input.is_open()) {
		cout << "Error opening the file" << endl;
		return 0;
	}

	input.seekg(0, ios::end);
	int len = (int)input.tellg();

	input.seekg(0, ios::beg);

	char* fileContents = new char[len + 1];

	input.read(fileContents, len);
	fileContents[len] = 0;

	input.close();
	
	// create an OpenGL vertex shader object/fragment shader object, set the source code, compile
	// create a new GLuint variable to store the index of the shader object
	GLuint iShaderObject = glCreateShader(shaderType); // Param: GL_VERTEX_SHADER || GL_FRAGMENT_SHADER
	glShaderSource(iShaderObject, 1, &fileContents, 0);
	glCompileShader(iShaderObject);
	
	delete[] fileContents; // all done with the string, we can delete it to prevent memory leaks

	// create a GLint variable to store the results of the shader compilation
	GLint compileResults;

	// check if the compilation succeeded or failed
	glGetShaderiv(iShaderObject, GL_COMPILE_STATUS, &compileResults);

	// if it succeeded, return the shader object index
	if (compileResults != 0) return iShaderObject;

	// if the shader did NOT compile successfully (compileResults == 0), we need to get the log info
	// create a GLint variable to store the results of the log length from the shader info log
	GLint logLen;

	glGetShaderiv(iShaderObject, GL_INFO_LOG_LENGTH, &logLen);

	// create an array to hold the log, instantiate it to the log length
	GLchar* logArr = new GLchar[logLen];

	glGetShaderInfoLog(iShaderObject, logLen, 0, logArr);

	// print out the shader info log 
	cout << logArr << endl;

	// delete shader and info log array and return 0
	glDeleteShader(iShaderObject);
	delete[] logArr;
	return 0;
}

bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile) {
	GLuint vShader = loadShader(vertexFile, GL_VERTEX_SHADER);

	// if vShader == 0, there was an error loading in the vertex shader file
	if (vShader == 0) return 0;

	GLuint fShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	// if fShader == 0, there was an error loading in the fragment shader file
	if (fShader == 0) return 0;

	// create shader program for OpenGL to use
	program = glCreateProgram();

	// attach both shaders we loaded to the program
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	// finally link the program
	glLinkProgram(program);

	// check if program linking succeeded or failed
	// create a GLint variable to store the results of the program linking
	GLint linkResults;

	glGetProgramiv(program, GL_LINK_STATUS, &linkResults);

	// if linking succeeded, return true
	if (linkResults != 0) return true;

	// if linking failed, get the log info
	GLint logLen;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

	// create an array to hold the log info
	GLchar* logArr = new GLchar[logLen];

	glGetProgramInfoLog(program, logLen, 0, logArr);

	// print out the log info
	cout << logArr << endl;

	// delete the program, log array and return false;
	glDeleteProgram(program);
	delete[] logArr;
	return false;
}