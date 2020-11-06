#version 410 core

layout (location = 0) in  vec3 position;
layout (location = 1) in  vec3 normal;
layout (location = 2) in  vec2 texCoords;

out vec3 Normal; 
out vec2 TexCoords;
out vec3 Color;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float size;
//uniform vec3 color;

void main()
{
	vec4 tmp = view * model * vec4(position,1.0f);
	float distance = length(vec3(tmp));

	float attenuation = inversesqrt(0.1 * distance);
	float pointsize = attenuation * size;

	gl_PointSize = pointsize;
	gl_Position = proj * view * model * vec4(position, 1.0f);

	//Color = color;
}