#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 0) uniform mat4 worldMatrix;
layout (location = 1) uniform mat4 cameraMat;

out vec3 FragLoc;
out vec2 FragUV;
out vec3 FragNorm;

out vec3 objectColor;
out vec3 lightColor;

void main(){
	gl_Position = cameraMat * worldMatrix * vec4(position, 1);

	FragLoc = vec3(worldMatrix * vec4(position, 1));
	FragUV = uv;
	FragNorm = normal;
	//FragNorm = mat3(transpose(inverse(worldMatrix))) * normal;

	objectColor = vec3(0.3, 1, 0.8);
	lightColor = vec3(1, 1, 1);
}
