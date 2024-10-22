#version 430

layout (location = 0) out vec4 FragColor;

uniform sampler2D myTexture;
uniform vec3 lightLoc;
uniform vec3 cameraLoc;

in vec3 FragLoc;
in vec2 FragUV;
in vec3 FragNorm;

in vec3 objectColor;
in vec3 lightColor;

void main(){
	vec3 L = normalize(FragLoc - lightLoc);
	vec3 E = normalize(cameraLoc - FragLoc);
	vec3 H = normalize(L + E);
	vec3 N = normalize(FragNorm);

	float diff = max(dot(L, N), 0);
	float amb = 0.5f;
	float spec = pow(max(dot(H, N), 0), 128);
	float lighting = amb + diff + spec;

	FragColor = vec4(lighting * objectColor * lightColor, 1);

	//vec3 norm = normalize(FragNorm);
	//vec3 lightDir = normalize(lightLoc - FragLoc);

	//float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * lightColor;

	//float specularStrength = 0.5;
	//vec3 viewDir = normalize(cameraLoc - FragLoc);

	//vec3 reflectDir = reflect(-lightDir, norm); // phong
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0); // phong

	//vec3 halfwayDir = normalize(lightDir + viewDir); // blinn-phong
	//float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); // blinn-phong

	//vec3 specular = specularStrength * spec * lightColor;

	//float ambientStrength = 0.3;
	//vec3 ambient = ambientStrength * lightColor;

	//vec3 lighting = ambient + diffuse + specular;

	//FragColor = vec4(lighting * objectColor, 1);

	//FragColor = texture(myTexture, uv);
}