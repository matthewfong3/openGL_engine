#include "Model.h"

Model::Model()
{
	VAO = 0;
	vertCount = 0;
}

Model::~Model(){

}

bool Model::buffer(string objFile)
{
	vector<glm::vec3> locations;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<VertInd> vertInds;

	// open and read in a file
	ifstream inFile;
	string line;

	inFile.open("models/" + objFile);

	if (inFile.is_open()) {
		while (getline(inFile, line)) {
			istringstream ss = istringstream(line);

			string type;
			float x, y, z;

			ss >> type;

			if (type == "v") {
				ss >> x >> y >> z;
				locations.push_back(glm::vec3(x, y, z));
			}
			else if (type == "vt") {
				ss >> x >> y;
				uvs.push_back(glm::vec2(x, y));
			}
			else if (type == "vn") {
				ss >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (type == "f") {
				for (int i = 0; i < 3; i++) {
					unsigned int a, b, c;
					char s1, s2;

					ss >> a >> s1 >> b >> s2 >> c;
					--a;
					--b;
					--c;
					vertInds.push_back({ a, b, c });
				}
			}
		}
		inFile.close();
	}

	vertCount = vertInds.size(); 

	// duplicate vertices into a single buffer
	vector<Vertex> vertBuffData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++) vertBuffData[i] = { locations[vertInds[i].locInd],
																	uvs[vertInds[i].uvInd],
																	normals[vertInds[i].normInd] };

	// set up GLuint variables to generate buffer (VBO) vertex buffer object
	GLuint VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// store the data in the buffer
	// glBufferData() copies specified data into currently bound buffer
	glBufferData(GL_ARRAY_BUFFER, // where to copy to
		sizeof(Vertex) * vertCount, // number of bytes to copy
		&vertBuffData[0], //where to copy from
		GL_STATIC_DRAW); // 'hint' to openGL - GL_STATIC_DRAW means we'll never change the data, lets openGL optimize how the buffer
	// is used internally. This is the fastest option

	// describe the buffer layout
	// glVertexAttribPointer() sets up a single vertex attribute
	// enable the attribute 
	glEnableVertexAttribArray(0); // Attribute index - 0 (location = 0  in vShader)

	// set up the attribute
	// locations
	glVertexAttribPointer(
		0, // attribute index - 0 in this case
		3, // number of components (x, y, z)
		GL_FLOAT, // type of data
		GL_FALSE, // should we normalize the data?
		sizeof(Vertex), // stride (bytes per vertex)
		0); // offset to this attribute

	glEnableVertexAttribArray(1); // (location = 1 in vShader) 

	// uvs
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3));

	glEnableVertexAttribArray(2);

	// normals
	glVertexAttribPointer(
		2,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0); // unbind when finished editing

	// OpenGL finally has our data, we can attempt to draw something now!

	// change the window color - set this once, no need to set each frame
	// defines the color to use when we clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

void Model::render()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
