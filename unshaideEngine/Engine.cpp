#include "Engine.h" 

// how to tell OpenGL to load and use multiple textures
// source: https://stackoverflow.com/questions/5681948/multiple-textures-opengl-glut-c

namespace {
	map<int, bool> keyIsDown;
	map<int, bool> keyWasDown;
}

Engine::Engine() {

}

Engine::~Engine() {
	// delete the texture when finished with them
	delete[] textures;
	delete[] objects;
}

bool Engine::init() {
	// initialize the window library
	if (glfwInit() == GL_FALSE) return false; // glfwInit() initializes GLFW, returns GL_TRUE, which is defined as 1 if it works
	// returns GL_FALSE, which is defined as 0 if it fails
	
	// create a windowed mode window
	this->GLFWwindowPtr = glfwCreateWindow(1920, 1080, "unshaide DSA1 engine", NULL, NULL);	// glfwCreateWindow() creates a window
	// with the parameters given, Returns the address of the GLFWwindow struct, Returns nullptr if it fails to create a window

	// Make it the currently active window, or quit
	if (this->GLFWwindowPtr != nullptr) glfwMakeContextCurrent(this->GLFWwindowPtr);
	else {
		glfwTerminate();
		return false;
	}

	// initialize GLEW, or quit
	if (glewInit() != GLEW_OK) { // glewInit() returns GLEW_OK, which is defined as 0, Returns something else if it fails
		glfwTerminate();
		return false;
	}

	// tell OpenGL to accept mouse clicks and keybaird inputs
	glfwSetMouseButtonCallback(this->GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(this->GLFWwindowPtr, keyCallback);

	// tell OpenGL to generate textures
	textures = new GLuint[4];
	glGenTextures(4, textures);

	// initialize our time variables
	currTime = glfwGetTime();
	prevTime = 0;
	deltaTime = 0;

	lightPos = glm::vec3(-0.3, -0.4, 1);

	// initialize camera settings
	camera = Camera();

	camera.mouseSens = 0.005; // mouse sensitivity used for turning camera with mouse 

	// view matrix
	camera.transform.location = { 0, 0, 2 };
	camera.transform.rotation = { 0, 0, 0 };

	// proj matrix
	zoom = 1.f;
	camera.width = 800;
	camera.height = 600;

	camera.fovy = 3.14159f * .4f / zoom;
	camera.aspect = (float)camera.width / (float)camera.height;
	camera.zNear = .01f;
	camera.zFar = 1000.f;

	projMat = glm::perspective(camera.fovy, camera.aspect, camera.zNear, camera.zFar);

	// hide the cursor
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// initialize model object
	model = Model();
	lightModel = Model();

	// initialize our collection of objects 
	objects = new Object[7];
	objects[0].transform.location = glm::vec3(-0.9, -0.4, 1);
	objects[0].transform.rotation = glm::vec3(0, 0, 0);
	objects[0].transform.scale = glm::vec3(0.3, 0.3, 0.3);
	objects[0].rb.mass = 1;
	objects[0].rb.velocity = glm::vec3(0, 0, 0);
	objects[0].rb.force = glm::vec3(0, 0, 0);
	objects[0].collider = SPHERE;

	objects[1].transform.location = glm::vec3(-0.3, -0.4, 1);
	objects[1].transform.rotation = glm::vec3(0, 0, 0);
	objects[1].transform.scale = glm::vec3(0.1, 0.1, 0.1);
	objects[1].rb.mass = 0.5;
	objects[1].rb.velocity = glm::vec3(0, 0, 0);
	objects[1].rb.force = glm::vec3(0, 0, 0);
	objects[1].collider = SPHERE;

	/*objects[2].transform.location = glm::vec3(-0.4, -0.4, 1);
	objects[2].transform.rotation = glm::vec3(0, 0, 0);
	objects[2].transform.scale = glm::vec3(0.1, 0.15, 0.1);
	objects[2].rb.mass = 2;
	objects[2].rb.velocity = glm::vec3(0, 0, 0);
	objects[2].rb.force = glm::vec3(0, 0, 0);
	objects[2].collider = AABB;

	objects[3].transform.location = glm::vec3(0.1, -0.4, 1);
	objects[3].transform.rotation = glm::vec3(0, 0, 0);
	objects[3].transform.scale = glm::vec3(0.1, 0.15, 0.1);
	objects[3].rb.mass = 2;
	objects[3].rb.velocity = glm::vec3(0, 0, 0);
	objects[3].rb.force = glm::vec3(0, 0, 0);
	objects[3].collider = AABB;

	objects[4].transform.location = glm::vec3(0.4, -0.4, 1);
	objects[4].transform.rotation = glm::vec3(0, 0, 0);
	objects[4].transform.scale = glm::vec3(0.1, 0.15, 0.1);
	objects[4].rb.mass = 2;
	objects[4].rb.velocity = glm::vec3(0, 0, 0);
	objects[4].rb.force = glm::vec3(0, 0, 0);
	objects[4].collider = AABB;

	objects[5].transform.location = glm::vec3(0, 1, 1);
	objects[5].transform.rotation = glm::vec3(0, 0, 0);
	objects[5].transform.scale = glm::vec3(2, 0.5, 1);
	objects[5].rb.mass = 2;
	objects[5].rb.velocity = glm::vec3(0, 0, 0);
	objects[5].rb.force = glm::vec3(0, 0, 0);

	objects[6].transform.location = glm::vec3(0, -1, 1);
	objects[6].transform.rotation = glm::vec3(0, 0, 0);
	objects[6].transform.scale = glm::vec3(2, 0.5, 1);
	objects[6].rb.mass = 2;
	objects[6].rb.velocity = glm::vec3(0, 0, 0);
	objects[6].rb.force = glm::vec3(0, 0, 0);*/

	// by default, all triangles are rendered
	// by convention, the vertices in triangles are listed in counter-clockwise order
	// if a triangle is listed in clockwise order, we must be looking at it from the back
	// we can skip drawing those with backface culling
	glEnable(GL_CULL_FACE);

	// by default, the last triangle through the pipeline draws on-top of all others
	// a better approach is to keep track of which fragments are closest to the screen
	// this is done by depth buffer
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool Engine::bufferModes()
{
	if(!model.buffer("sphere.obj")) return false;
	if (!lightModel.buffer("sphere.obj")) return false;
}

bool Engine::gameLoop() {
	// loop until the user closes the window
	while (!glfwWindowShouldClose(this->GLFWwindowPtr)) { // return true if the window was told to close during the last frame
		// upload a uniform vec3 location for lighting to shader
		GLint lightLoc = glGetUniformLocation(sm.getProgram(), "lightLoc");
		glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);
		
		// update time values
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;
		
		// update physical simulation
		// draw buffered models
		// process input/window events

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the canvas -	can pass a series of buffers as parameters, clears a buffer (color buffer, depth buffer, etc.)

		// update physical simulation
		update();

		//if (collides(objects[0], objects[1])) cout << "collision detected with coin" << endl;
		//for(int i = 2; i < 5; i++ ) if (collides(objects[0], objects[i])) cout << "collision detected with block" << endl;
		
		// update camera logic
		updateCamera();
		// upload a uniform vec3 location for camera to shader
		GLint cameraLoc = glGetUniformLocation(sm.getProgram(), "cameraLoc");
		glUniform3f(cameraLoc, camera.transform.location.x, camera.transform.location.y, camera.transform.location.z);

		// draw model
		draw(0, 4, model);

		// draw light source
		//draw(1, 3, lightModel);

		// draw blocks
		//for (int i = 2; i <= 4; i++) draw(i, 2);

		// draw bricks
		//for (int i = 5; i < 7; i++) draw(i, 3);

		// swap the front (what the screen displays) and the back (what openGL draws to) buffers
		glfwSwapBuffers(this->GLFWwindowPtr);

		// process queued window, mouse & keyboard callback events
		// update input
		keyWasDown = keyIsDown;
		glfwPollEvents(); // events queue up during a frame (dragging the screen around, pressing a key). once per frame, the events must
		// be processed 
		
		// if the escape key is pressed, quit the engine
		if(keyIsDown[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(this->GLFWwindowPtr, GL_TRUE);
	}

	glfwTerminate(); // must be called before your engine closes, or you will have memory leaks or worse

	return true;
}

bool Engine::useShaders() {
	if (!sm.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl")) return false;
	// once we verified that the program linked correctly
	// we can tell OpenGL to use the shader program 
	// ALL FUTURE DRAW COMMANDS WILL USE THIS SHADER PROGRAM (your vertex and fragment shaders)
	glUseProgram(sm.getProgram());
	return true;
}

bool Engine::loadTexture(GLuint texture, const char* filename)
{
	// if using stbi_image to load textures
	/*int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/TestTexture.png", &width, &height, &nrChannels, 0);

	if (!data) return false;*/

	// use FreeImage to load texture and check if it worked
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(filename, 0), filename);

	// load failed
	if (!image) return false;

	// use FreeImage to convert the original image to a 32bit bmp, then unload the original image from memory
	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);

	FreeImage_Unload(image);

	// generate and bind a new texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// upload texture bytes
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB_ALPHA,
		FreeImage_GetWidth(image32Bit), // width - if using stbi_image to load textures
		FreeImage_GetHeight(image32Bit), // height - if using stbi_image to load textures
		0,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit)); // data - if using stbi_image to load textures

	// set min filter to linear instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// clear the texture from RAM (NOT VRAM). FREEIMAGE took it from the hard drive, stored it in RAM, sent it to GPU
	// we no longer need it in RAM
	FreeImage_Unload(image32Bit);
	//stbi_image_free(data); // if using stbi_image to load textures

	// unbind the texture when finished uploading
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Engine::loadTextures() {
	if (!loadTexture(textures[0], "textures/mario.png")) return false;
	if (!loadTexture(textures[1], "textures/coin.png")) return false;
	if (!loadTexture(textures[2], "textures/block.jpg")) return false;
	if (!loadTexture(textures[3], "textures/bricks.png")) return false;
	if (!loadTexture(textures[4], "textures/TestTexture.png")) return false;

	return true;
}

void Engine::draw(int i, GLuint texture, Model& model)
{
	objects[i].transform.worldMatrix =
		glm::translate(objects[i].transform.location) *
		glm::yawPitchRoll(objects[i].transform.rotation.x, objects[i].transform.rotation.y, objects[i].transform.rotation.z) *
		glm::scale(objects[i].transform.scale);

	glUniformMatrix4fv(0, 1, GL_FALSE,
		&objects[i].transform.worldMatrix[0][0]);

	// render game objects
	//glBindTexture(GL_TEXTURE_2D, textures[texture]);
	model.render();
	//glBindVertexArray(0); // unbind an object after drawing it
}

bool Engine::collides(const Object& o1, const Object& o2) {
	if (o1.collider == COLLIDERLESS && o2.collider == COLLIDERLESS) return false;

	if (o1.collider == SPHERE && o2.collider == SPHERE) {
		double distance = pow(o1.transform.location.x - o2.transform.location.x, 2) +
							pow(o1.transform.location.y - o2.transform.location.y, 2) +
							pow(o1.transform.location.z - o2.transform.location.z, 2);

		double sum = pow(o1.transform.scale.x + o2.transform.scale.x, 2);

		if (distance > sum) return false;
	}
	else if (o1.collider == AABB && o2.collider == AABB) {
		double sepX = abs(o1.transform.location.x - o2.transform.location.x);
		double sepY = abs(o1.transform.location.y - o2.transform.location.y);
		double sepZ = abs(o1.transform.location.z - o2.transform.location.z);

		if (sepX > (o1.transform.scale.x + o2.transform.scale.x) ||
			sepY > (o1.transform.scale.y + o2.transform.scale.y) ||
			sepZ > (o1.transform.scale.z + o2.transform.scale.z)) return false;
	}
	else {
		double distance = 0;

		if (o2.transform.location.x < o1.transform.location.x) distance += o1.transform.location.x - o2.transform.location.x;
		else if (o2.transform.location.x > o1.transform.location.x + o1.transform.scale.x) distance += o2.transform.location.x - o1.transform.location.x + o1.transform.scale.x;

		if (o2.transform.location.y < o1.transform.location.y) distance += o1.transform.location.y - o2.transform.location.y;
		else if (o2.transform.location.y > o1.transform.location.y + o1.transform.scale.y) distance += o2.transform.location.y - o1.transform.location.y + o1.transform.scale.y;
		
		if (o2.transform.location.z < o1.transform.location.z) distance += o1.transform.location.z - o2.transform.location.z;
		else if (o2.transform.location.z > o1.transform.location.z + o1.transform.scale.z) distance += o2.transform.location.z - o1.transform.location.z + o1.transform.scale.z;

		distance = pow(distance, 2);

		if (distance > pow(o2.transform.scale.x, 2)) return false;
	}

	return true;
}

void Engine::update() {
	if (keyIsDown[GLFW_KEY_RIGHT]) objects[0].rb.force = glm::vec3(1, 0, 0); // objects[0].rb.velocity += glm::vec3(1, 0, 0) * (float)deltaTime;
	if (keyIsDown[GLFW_KEY_LEFT]) objects[0].rb.force = glm::vec3(-1, 0, 0); // objects[0].rb.velocity += glm::vec3(-1, 0, 0) * (float)deltaTime;

	if (objects[0].rb.force != glm::vec3()) objects[0].rb.force = glm::normalize(objects[0].rb.force) * 1.f;

	objects[0].rb.velocity += (objects[0].rb.force / objects[0].rb.mass) * (float)deltaTime;

	if (objects[0].rb.velocity != glm::vec3()) objects[0].rb.velocity = glm::normalize(objects[0].rb.velocity) * 1.f;

	objects[0].transform.location += objects[0].rb.velocity * (float)deltaTime;

	objects[0].rb.velocity *= 0;
	objects[0].rb.force *= 0;
}

void Engine::updateCamera()
{
	// turn with mouse
	glfwGetCursorPos(GLFWwindowPtr, &camera.mouseX, &camera.mouseY);

	camera.transform.rotation.y -= camera.mouseSens * (camera.mouseX - camera.width * 0.5f); // yaw
	camera.transform.rotation.x -= camera.mouseSens * (camera.mouseY - camera.height * 0.5f); // pitch
	camera.transform.rotation.x = glm::clamp(camera.transform.rotation.x, -0.5f * 3.14159f, 0.5f * 3.14159f);

	glfwSetCursorPos(GLFWwindowPtr, camera.width * 0.5f, camera.height * 0.5f);

	// fps controls
	// updating the camera's location with velocity
	rotMat = (glm::mat3)glm::yawPitchRoll(camera.transform.rotation.y, camera.transform.rotation.x, camera.transform.rotation.z);

	if (keyIsDown[GLFW_KEY_A]) camera.rb.velocity += rotMat * glm::vec3(-1, 0, 0) * (float)deltaTime;
	if (keyIsDown[GLFW_KEY_D]) camera.rb.velocity += rotMat * glm::vec3(1, 0, 0) * (float)deltaTime;
	if (keyIsDown[GLFW_KEY_W]) camera.rb.velocity += rotMat * glm::vec3(0, 0, -1) * (float)deltaTime;
	if (keyIsDown[GLFW_KEY_S]) camera.rb.velocity += rotMat * glm::vec3(0, 0, 1) * (float)deltaTime;
	if(keyIsDown[GLFW_KEY_E]) camera.rb.velocity += rotMat * glm::vec3(0, -1, 0) * (float)deltaTime;
	if (keyIsDown[GLFW_KEY_Q]) camera.rb.velocity += rotMat * glm::vec3(0, 1, 0) * (float)deltaTime;

	if (camera.rb.velocity != glm::vec3()) camera.rb.velocity = glm::normalize(camera.rb.velocity) * 1.f;

	camera.transform.location += camera.rb.velocity * (float)deltaTime;

	// reset velocity to zero
	camera.rb.velocity *= 0;

	// move or turn camera
	eye = camera.transform.location;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);
	viewMat = glm::lookAt(eye, center, up);

	// calculate the camera's world matrix
	camera.worldMatrix = projMat * viewMat;

	// upload uniform camera data matrix
	glUniformMatrix4fv(1, 1, GL_FALSE, &camera.worldMatrix[0][0]);
}

void mouseClick(GLFWwindow* windowPtr, int button, int action, int mods)
{
	keyIsDown[button] = action;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}
