#version 400 core

in vec3 ourColor;
in vec3 normalWorld;
in vec3 fragPos;

#define MAX_LIGHTS 32
uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec3 ambientColor;

out vec4 FragColor;

void main()
{
	vec3 n = normalize(normalWorld);
	vec3 l = normalize(lightPos[0] - fragPos);
	
	float diff = max(dot(n, l), 0.0);
	vec3 diffuse = diff * lightColor[0];
	vec3 ambient = ambientColor;
	
	FragColor = vec4(ourColor * (ambient + diffuse), 1.0);
}