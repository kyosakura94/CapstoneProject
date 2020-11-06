#include "SpriteSurface.h"

SpriteSurface::SpriteSurface(string nametexture_, float scale_, float angle_, vec4 tintColor_, GLuint shaderProgram_) :
    angle(0), scale(vec2(1.0f, 1.0f)), tintColor(1,1,1,1),
	VAO(0), VBO(0), modelLoc(0), viewLoc(0), projLoc(0), textureLoc(0)
{
	shaderProgram = shaderProgram_;

	Vertex2D vertA;
	vertA.position = vec2(-0.5f, 0.5f);
	vertA.texCoords = vec2(0.0f, 0.0f);
	vertexList.push_back(vertA);

	Vertex2D vertB;
	vertB.position = vec2(0.5f, 0.5f);
	vertB.texCoords = vec2(1.0f, 0.0f);
	vertexList.push_back(vertB);
	
	Vertex2D vertC;
	vertC.position = vec2(-0.5f, -0.5f);
	vertC.texCoords = vec2(0.0f, 1.0f);
	vertexList.push_back(vertC);

	Vertex2D vertD;
	vertD.position = vec2(0.5f, -0.5f);
	vertD.texCoords = vec2(1.0f, 1.0f);
	vertexList.push_back(vertD);

	scale = vec2(scale_);
	angle = angle_;
	tintColor = tintColor_;

	textureID = LoadTexture(nametexture_);

	height = TextureHandler::getInstance()->GetTextureData(nametexture_)->height;
	width = TextureHandler::getInstance()->GetTextureData(nametexture_)->width;

	GenerateBuffers();
}

SpriteSurface::~SpriteSurface()
{
	vertexList.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void SpriteSurface::Draw(Camera* camera_, vec2 position_)
{
	//texture
	glUniform1i(textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	mat4 model;
	model = glm::translate(model, vec3(position_.x, position_.y, 0.0f));
	model = glm::rotate(model, angle, vec3(0,0,1));
	model = glm::scale(model, vec3(width * scale.x, height * scale.y, 1.0f));



	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetOrthographic()));
	glUniform4fv(colorLoc, 1, glm::value_ptr(vec4(tintColor.r, tintColor.g, tintColor.b, tintColor.a)));

	glBindVertexArray(VAO);

	//for (int i = 0; i < vertexList.size(); i++)
	//{
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexList.size());
	//}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexList.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

float SpriteSurface::getWidth()
{
	return width;
}

float SpriteSurface::getHeight()
{
	return height;
}

void SpriteSurface::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex2D), &vertexList[0], GL_STATIC_DRAW);
	
	//POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

	//TEXTURECORDINATE
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, texCoords));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	colorLoc = glGetUniformLocation(shaderProgram, "tintColour");
	textureLoc = glGetUniformLocation(shaderProgram, "inputTexture");
}

GLuint SpriteSurface::LoadTexture(string filename_)
{
	GLuint currentTexture = TextureHandler::getInstance()->GetTexture(filename_);
	if (currentTexture == 0)
	{
		TextureHandler::getInstance()->CreateTexture(filename_,
			"./Resources/Textures/" + filename_ + ".png");
		currentTexture = TextureHandler::getInstance()->GetTexture(filename_);
	}
	return currentTexture;
}
