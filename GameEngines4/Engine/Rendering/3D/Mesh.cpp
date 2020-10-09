#include "Mesh.h"

Mesh::Mesh(SubMesh subMesh_, GLuint shaderProgram_):
VAO(0), VBO(0), 
modelLoc(0),viewLoc(0), projLoc(0), 
diffuseMap(0), shininess(0), transparency(0), ambient(0), diffuse(0), specular(0),
viewPosition(0), lightPosition(0), ambientValue(0), diffuseValue(0), lightColour(0)
{
	//dereference to get the context of vecter
	shaderProgram = shaderProgram_;
	subMesh = subMesh_;
	GenerateBuffers();
}

Mesh::~Mesh()
{
	subMesh.vertexList.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::Render(Camera* camera_, std::vector<glm::mat4> instances)
{
	glUniform1i(diffuseMap, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));
	
	glUniform1f(shininess, subMesh.material.shininess);
	glUniform1f(transparency, subMesh.material.transparency);
	//Ka
	glUniform3f(ambient, subMesh.material.ambient.x, subMesh.material.ambient.y, subMesh.material.ambient.z);
	//Kd
	glUniform3f(diffuse, subMesh.material.diffuse.x, subMesh.material.diffuse.y, subMesh.material.diffuse.z);
	//Ks
	glUniform3f(specular, subMesh.material.spectular.x, subMesh.material.spectular.y, subMesh.material.spectular.z);


	glUniform3f(viewPosition, 
		camera_->GetPosition().x,
		camera_->GetPosition().y, 
		camera_->GetPosition().z);

	glUniform3f(lightPosition,
		camera_->GetLightSources()[0]->GetLightPosition().x,
		camera_->GetLightSources()[0]->GetLightPosition().y,
		camera_->GetLightSources()[0]->GetLightPosition().z);

	glUniform3f(lightColour,
		camera_->GetLightSources()[0]->GetLightColor().x,
		camera_->GetLightSources()[0]->GetLightColor().y,
		camera_->GetLightSources()[0]->GetLightColor().z);

	glUniform1f(ambientValue, camera_->GetLightSources()[0]->GetAmbientValue());
	glUniform1f(diffuseValue, camera_->GetLightSources()[0]->GetDiffuseValue());


	glBindVertexArray(VAO);

	for (int i = 0; i < instances.size(); i++)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(instances[i]));
		glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

SubMesh Mesh::getSubmesh()
{
	return subMesh;
}

void Mesh::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex), &subMesh.vertexList[0], GL_STATIC_DRAW);
	//POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	//TEXTURECORDINATE
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords ));

	//COLOUR
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	//textureLoc = glGetUniformLocation(shaderProgram, "inputTexture");

	diffuseMap = glGetUniformLocation(shaderProgram, "material.diffuseMap");

	shininess = glGetUniformLocation(shaderProgram, "material.shininess");
	transparency = glGetUniformLocation(shaderProgram, "material.transparency");

	ambient = glGetUniformLocation(shaderProgram, "material.ambient");
	diffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
	specular = glGetUniformLocation(shaderProgram, "material.specular");

	viewPosition = glGetUniformLocation(shaderProgram, "viewPosition");
	lightPosition = glGetUniformLocation(shaderProgram, "light.lightPos");
	lightColour = glGetUniformLocation(shaderProgram, "light.lightColor");
	ambientValue = glGetUniformLocation(shaderProgram, "light.ambient");
	diffuseValue = glGetUniformLocation(shaderProgram, "light.diffusevalue");





}
