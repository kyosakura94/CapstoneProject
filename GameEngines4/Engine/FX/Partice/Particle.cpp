#include "Particle.h"

Particle::Particle(GLuint shaderProgram_, GLuint textureID_, vec3 colour_)
{
	shaderProgram = shaderProgram_;
	textureID = textureID_;
	size = 1;
	colour = colour_;

	GenerateBuffers();
}

Particle::Particle(GLuint shaderProgram_, vec3 position_) : Position(vec3(2,1,0)), colour(vec3(1,0,0)), Velocity(vec3(0.05f, 0.05f, 0.0f)), lifeTime(200.f)
{
	Position = position_;
	shaderProgram = shaderProgram_;
	size = 5.0f;
	GenerateBuffers();
}

Particle::~Particle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Particle::Render(Camera* camera_)
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//texture
	//glUniform1i(textureLoc, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureID);

	mat4 model;
	model = glm::translate(model, vec3(Position.x, Position.y, Position.z));

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, 1, &Position, GL_STATIC_DRAW);

	////POSITION
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 1, (GLvoid*)0);


	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));

	glUniform3fv(colorLoc, 1, glm::value_ptr(vec3(colour)));


	glUniform1f(pointSize, size);

	glBindVertexArray(VAO);

	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);


	//glBindTexture(GL_TEXTURE_2D, 0);

}

void Particle::Update(const float timeDelta_)
{
	lifeTime -= timeDelta_;
	Position += (timeDelta_ * Velocity);
}

float Particle::getLifetime()
{
	return lifeTime;
}

void Particle::setOriginal(vec3 pos_)
{
	Position = pos_;
	json j;

}

void Particle::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindVertexArray(0);

	//Render point
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	colorLoc = glGetUniformLocation(shaderProgram, "color");

	//textureLoc = glGetUniformLocation(shaderProgram, "inputTexture");

	pointSize = glGetUniformLocation(shaderProgram, "size");

	//Render plane need set up VBO - 4 vertex - same as sprite surface
}
