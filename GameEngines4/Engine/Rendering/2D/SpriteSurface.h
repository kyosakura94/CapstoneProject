#ifndef SPRITESURFACE_H
#define SPRITESURFACE_H

#include <glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Camera/Camera.h"
#include "../../Graphics/TextureHandler.h"


struct  Vertex2D
{
	vec2 position;
	vec2 texCoords;
};
struct SubSprite 
{

};

class SpriteSurface
{
public:
	SpriteSurface(string nametexture_, float scale_, float angle_, vec4 tintColor_ , GLuint shaderProgram_);
	~SpriteSurface();
	void Draw(Camera* camera_, vec2 position_);
	float getWidth();
	float getHeight();
private:
	void GenerateBuffers();
	vec2 scale;
	float angle, width, height;
	string name;
	vec4 tintColor;
	vector<Vertex2D> vertexList;
	GLuint VAO, VBO;
	GLuint textureID;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc, textureLoc, colorLoc;
	GLuint viewPosition, lightPosition, ambientValue, diffuseValue, lightColour;
	GLuint diffuseMap, shininess, transparency, ambient, diffuse, specular;

	GLuint LoadTexture(string filename_);
};
#endif // !SPRITESURFACE_H



