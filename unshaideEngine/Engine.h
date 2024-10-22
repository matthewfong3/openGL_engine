#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include "stb_image.h"
#include <vector>
#include <map>
#include <cmath>
#include "ShaderManager.h"
#include "Model.h"
using namespace std;

enum Collider {
	COLLIDERLESS,
	AABB, //axis-aligned bounding box
	SPHERE,
};

struct Rigidbody {
	float mass;
	glm::vec3 velocity;
	glm::vec3 force;
};

struct Transform {
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 worldMatrix;
};

struct Object {
	Transform transform;
	string filename;
	Rigidbody rb;
	Collider collider;
};

/*struct Vertex {
	glm::vec3 loc;
	glm::vec2 uv;
};*/

struct Camera {
	float fovy;
	int width;
	int height;
	float aspect;
	float zNear;
	float zFar;
	Transform transform;
	Rigidbody rb;
	glm::mat4 worldMatrix;
	float mouseSens;
	double mouseX;
	double mouseY;
};

class Engine
{
private:
	GLFWwindow* GLFWwindowPtr;
	GLuint* textures;
	Model model;
	Model lightModel;
	glm::vec3 lightPos;
	
	ShaderManager sm;
	Object* objects;
	double currTime;
	double prevTime;
	double deltaTime;

	Camera camera;

	// view matrix
	glm::mat3 rotMat;
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	glm::mat4 viewMat;

	// proj matrix
	float zoom;
	glm::mat4 projMat;

	bool loadTexture(GLuint texture, const char* filename);
	void draw(int i, GLuint texture, Model& model);
	void update();
	bool collides(const Object& o1, const Object& o2);
	void updateCamera();
public:
	Engine();
	~Engine();

	bool init();
	bool bufferModes();
	bool gameLoop();
	bool useShaders();
	bool loadTextures();
};

void mouseClick(GLFWwindow* windowPtr, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);