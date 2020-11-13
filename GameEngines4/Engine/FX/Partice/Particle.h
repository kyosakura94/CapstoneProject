#ifndef PARTICLE_H
#define PARTICLE_H


#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "../../Camera/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>

using json = nlohmann::json;
class Particle
{
public:

	Particle(GLuint shaderProgram_, GLuint textureID_, vec3 colour_);
	Particle(GLuint shaderProgram_, vec3 position_);
	~Particle();
	void Render(Camera * camera_);
	void Update(const float timeDelta_);
	float getLifetime();
	void setOriginal(vec3 pos_);
	void setVelocity(vec3 vel_) { Velocity = vel_; };
	void setSize(float size_) { size = size_; };
	void setColor(vec3 color_) { colour = color_; };

private:
	void GenerateBuffers();
	GLuint modelLoc, viewLoc, projLoc, textureLoc, colorLoc, pointSize;
	GLuint shaderProgram;
	GLuint textureID;
	GLuint VAO, VBO;
	vec3 Position;
	vec3 Velocity;
	vec3 colour;
	float lifeTime;
	float size;
};


#endif // !PARTICLE_H

