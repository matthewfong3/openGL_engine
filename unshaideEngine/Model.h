#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

struct Vertex {
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd {
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

class Model
{
private:
	GLuint VAO; // vertex array object
	unsigned int vertCount;		

public:
	Model();
	~Model();

	bool buffer(string objFile);
	void render();
};

