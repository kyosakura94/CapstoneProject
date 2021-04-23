#include "Model.h"

Model::Model()
{
}

Model::Model(const string& objFilepath_, const string& matFilepath_, GLuint shaderProgram_, RendererType rendererType_): subMesh(std::vector<OpenGLMesh*>()), shaderProgram(0),
modelInstances(vector<mat4>()), obj(nullptr)
{
	subMesh.reserve(10);
	modelInstances.reserve(5);
	shaderProgram = shaderProgram_;
	rendererType = rendererType_;
	obj = new LoadOBJModel();
	obj->LoadModel(objFilepath_, matFilepath_);
	modelName = obj->getModelName();

	for (auto m : obj->getvertercies())
	{
		vertercies.push_back(m);
	}


	this->LoadModel();
}


Model::~Model()
{
	if (subMesh.size() > 0)
	{
		for (auto m : subMesh)
		{
			delete m;
			m = nullptr;
		}

		subMesh.clear();
	}
}

void Model::Render(Camera *camera_, string tag)
{
	for (auto m : subMesh)
	{
		if (this->getJointCount() != 0)
		{
			m->AnimatedRender(camera_, modelInstances, this->getJointCount(), getCurrentPose());
		}
		else
		{
			m->Render(camera_, modelInstances);
		}
	}
}



void Model::Render(Camera* camera_, string tag, GLuint shadeUsing_, unsigned int depthMap, mat4 lightmatrix)
{
	for (auto m : subMesh)
	{
		m->shadowGenerateBuffers(shadeUsing_);

		if (this->getJointCount() != 0)
		{
			m->AnimatedRender(camera_, modelInstances, this->getJointCount(), getCurrentPose(), depthMap, lightmatrix);
		}
		else
		{
			m->Render(camera_, modelInstances, depthMap, lightmatrix);
		}
	}
}

void Model::ShadowRender(Camera* camera_, GLuint shaderProgram_)
{
	for (auto m : subMesh)
	{
		m->shadowGenerateBuffers(shaderProgram_);
		m->ShadowRender(camera_, modelInstances);
	}
}

void Model::Render(Camera* camera_, GLuint shaderProgram_)
{
	for (auto m : subMesh)
	{
		if (this->getJointCount() != 0)
		{
			m->AnimatedRender(camera_, modelInstances, this->getJointCount(), getCurrentPose());
		}
		else
		{
			m->Render(camera_, modelInstances);
		}
	}
}

void Model::AddMesh(OpenGLMesh* mesh_)
{
	subMesh.push_back(mesh_);

}

int Model::CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat rotationquaternion_)
{
	modelInstances.push_back(GetTransform(position_, angle_, rotation_, scale_, rotationquaternion_));

	return modelInstances.size() - 1;
}

void Model::UpdateInstance(int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat quaternionRotation_)
{
	modelInstances[index_] = GetTransform(position_, angle_, rotation_, scale_, quaternionRotation_);

}

void Model::RemoveInstance(int index_)
{
	if (modelInstances.size() <= 1)
	{
		modelInstances.erase(modelInstances.begin() + index_);
	}
	else
	{
		quat quaternionRotation_;
		modelInstances[index_] = GetTransform(vec3(0), 0, vec3(0), vec3(0), quaternionRotation_);
	}
	
}

void Model::ChangeProgram(GLuint program_)
{
	for (auto m : subMesh)
	{
		m->useProgram(program_);
	}
}

BoundingBox Model::GetBoundingBox()
{
	return box;
}

GLuint Model::getShaderProgram()
{
	return shaderProgram;
}

mat4 Model::GetTransform(int index_) const
{

	return modelInstances[index_];
}


int Model::AABBandFrustum(Camera * camera)
{
	int result = INSIDE;
	for (int i = 0; i < 6; i++)
	{
		float d = camera->GetPlane()[i]->d;
		camera->GetPlane()[i]->normalize();
		vec3 nP = vec3(camera->GetPlane()[i]->x, camera->GetPlane()[i]->y, camera->GetPlane()[i]->z);
		//X
		{
			vec3 xAxis(box.transform[0].x, box.transform[0].y, box.transform[0].z);
			vec3 negativePoint = cross(xAxis, nP);
			if (float m = (dot(nP, negativePoint) + d) < 0)
			{
				return OUTSIDE;
			}
			vec3 positivePoint = cross(xAxis, nP);
			if (float n = (dot(nP, positivePoint) + d) < 0)
			{
				result = INTERSECT;
			}
		}
		//Y
		{
			vec3 yAxis(box.transform[1].x, box.transform[1].y, box.transform[1].z);
			vec3 negativePoint = cross(yAxis, nP);
			if ((dot(nP, negativePoint) + d) < 0)
			{
				return OUTSIDE;
			}
			vec3 positivePoint = cross(yAxis, nP);
			if ((dot(nP, positivePoint) + d) < 0)
			{
				result = INTERSECT;
			}
		}
		//Z
		{
			vec3 zAxis(box.transform[2].x, box.transform[2].y, box.transform[2].z);
			vec3 negativePoint = cross(zAxis, nP);
			if ((dot(nP, negativePoint) + d) < 0)
			{
				return OUTSIDE;
			}
			vec3 positivePoint = cross(zAxis, nP);
			if ((dot(nP, positivePoint) + d) < 0)
			{
				result = INTERSECT;
			}
		}
	}
	return (result);

	{
	/*bool result;
	for (int i = 0; i < 6; i++)
	{
		vec3 nP = vec3(camera->GetPlane()[i]->x, camera->GetPlane()[i]->y, camera->GetPlane()[i]->z);

		{
			vec3 xAxis(box.transform[0].x, box.transform[0].y, box.transform[0].z);
			float Denominator = xAxis.x * nP.y - xAxis.y * nP.x;
			if (Denominator == 0.0f)
			{
				return false;
			}

			vec3 crossVector = cross(xAxis, nP);
			if (crossVector.length() == 0.0f)
			{
				return false;
			}
		}

		{
			vec3 yAxis(box.transform[1].x, box.transform[1].y, box.transform[1].z);
			float Denominator = yAxis.x * nP.y - yAxis.y * nP.x;
			if (Denominator == 0.0f)
			{
				return false;
			}

			vec3 crossVector = cross(yAxis, nP);
			if (crossVector.length() == 0.0f)
			{
				return false;
			}
		}

		{
			vec3 zAxis(box.transform[2].x, box.transform[2].y, box.transform[2].z);
			float Denominator = zAxis.x * nP.y - zAxis.y * nP.x;
			if (Denominator == 0.0f)
			{
				return false;
			}

			vec3 crossVector = cross(zAxis, nP);
			if (crossVector.length() == 0.0f)
			{
				return false;
			}
		}

	}

	return true;*/
	
		/*float m, n;
		int result = 1;

		for (int i = 0; i < 6; i++)
		{
			vec3 nP = cross(vec3(camera->GetPlane()[i]->x, camera->GetPlane()[i]->y, camera->GetPlane()[i]->z), box.maxVert);
			vec3 nN = cross(vec3(camera->GetPlane()[i]->x, camera->GetPlane()[i]->y, camera->GetPlane()[i]->z), box.minVert);

			m = (camera->GetPlane()[i]->x * nN.x)
				+ (camera->GetPlane()[i]->y * nN.y)
				+ (camera->GetPlane()[i]->z * nN.z);

			if (m > -camera->GetPlane()[i]->d)
				result = 0;

			n = (camera->GetPlane()[i]->x * nP.x) +
				(camera->GetPlane()[i]->y * nP.y) +
				(camera->GetPlane()[i]->z * nP.z);

			if (n > -camera->GetPlane()[i]->d)
				result = 1;
		}
		return result;*/

		/*for (int i = 0; i < 6; i++)
		{
			vec4 plane = vec4(camera->GetPlane()[i]->x, camera->GetPlane()[i]->y, camera->GetPlane()[i]->z, camera->GetPlane()[i]->d);
			int out = 0;
			out += ((dot(plane, vec4(box.minVert, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.maxVert.x, box.minVert.y, box.minVert.z, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.minVert.x, box.maxVert.y, box.minVert.z, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.maxVert.x, box.maxVert.y, box.minVert.z, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.minVert.x, box.minVert.y, box.maxVert.z, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.maxVert.x, box.minVert.y, box.maxVert.z, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.minVert.x, box.maxVert.y, box.maxVert.x, 1.0f)) < 0.0) ? 1 : 0);
			out += ((dot(plane, vec4(box.maxVert, 1.0f)) < 0.0) ? 1 : 0);
			if (out == 8)
				return false;
		}

		return true;*/
	
	}

}

//LoadOBJModel* Model::getLoadOBJModel()
//{
//	return obj;
//}

vector<vec3> Model::getvertercies(int index_)
{
	worldVertecies.clear();
	for (auto m : vertercies)
	{
		vec4 worldPosition = GetTransform(index_) * vec4(m, 1.0f);
		worldVertecies.push_back(vec3(worldPosition.x, worldPosition.y, worldPosition.z));
	}

	return worldVertecies;
}

glm::mat4 Model::GetTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat quaternionRotation_) const
{
	//float elapsedTime = (float)SDL_GetTicks() / 1000;
	//quat rotateQuat;

	//vec3 EulerAngles(0, -elapsedTime * 3.0f, 0);
	//rotateQuat = quat(EulerAngles);

	glm::mat4 model;

	mat4 translationMatrix = translate(model, position_);
	mat4 rotationmatrix = toMat4(quaternionRotation_);
	//mat4 rotationmatrix = rotate(model, angle_, rotation_);

	mat4 scaleMatrix = scale(model, scale_);
	mat4 finalmodel = translationMatrix * rotationmatrix * scaleMatrix;

	//model = translate(model, position_);
	//model = rotate(model, angle_, rotation_);
	//model = scale(model, scale_);


	return finalmodel;
	//return model;
}

void Model::LoadModel()
{
	switch (rendererType)
	{
	case RendererType::OPENGL:

		for (int i = 0; i < obj->GetSubMeshes().size(); i++)
		{
			subMesh.push_back(new OpenGLMesh(obj->GetSubMeshes()[i], shaderProgram));
		}
		break;

	default:
		for (int i = 0; i < obj->GetSubMeshes().size(); i++)
		{
			subMesh.push_back(new OpenGLMesh(obj->GetSubMeshes()[i], shaderProgram));
		}
		break;
	}

	box = obj->GetBoundingBox();

	//delete obj;
	//obj = nullptr;
}


