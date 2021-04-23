#version 410 core

in vec3 Normal; 
in vec2 TexCoords;
in vec3 Color;

uniform vec3 color;
out vec4 fragColor;

void main()
{
	fragColor = vec4(color, 1.0f);
}