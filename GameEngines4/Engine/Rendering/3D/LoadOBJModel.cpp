#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel() : currentMaterial(Material())
{
	vertercies.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	nomalIndices.reserve(200);
	textureIndices.reserve(200);
	meshVerterices.reserve(200);
	subMeshes.reserve(10);
}


LoadOBJModel::~LoadOBJModel()
{
	OnDestroy();
}

void LoadOBJModel::LoadModel(const std::string & objFilePath)
{
	float maxX = -9999.99f;
	float maxY = -9999.99f;
	float maxZ = -9999.99f;
	float minX = 9999.99f;
	float minY = 9999.99f;
	float minZ = 9999.99f;

	ifstream in(objFilePath.c_str(), ios::in);
	if (!in)
	{
		Debug::Error("Cannot open OBJ file:" + objFilePath, "LoadOBJModel.cpp", __LINE__);
		return;
	}
	string line;
	while (getline(in, line))
	{
		//VERTEX DATA
		if (line.substr(0, 2) == "v ")
		{
			istringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;
			vertercies.push_back(vec3(x, y, z));
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
			if (z > maxZ)
			{
				maxZ = z;
			}
			if (x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
			if (z < minZ)
			{
				minZ = z;
			}
			box.maxVert = vec3(maxX, maxY, maxZ);
			box.minVert = vec3(minX, minY, minZ);
		}
		//NORMAL DATA
		else if (line.substr(0, 3) == "vn ")
		{
			istringstream v(line.substr(3));
			float x, y, z;
			v >> x >> y >> z;
			normals.push_back(vec3(x, y, z));
		}
		//TEXTURE COODINATE DATA
		else if (line.substr(0, 3) == "vt ")
		{
			istringstream v(line.substr(3));
			float x, y, z;
			v >> x >> y >> z;
			textureCoords.push_back(vec2(x, y));

		}
		//FACE DATA
		else if (line.substr(0, 2) == "f ")
		{
			istringstream v(line.substr(2));
			string word;
			int x[3], y[3], z[3];
			int i = 0;
			char sep;

			while (v>>word)
			{
				istringstream ss(word);
				ss >> x[i] >> sep >> y[i] >> sep >> z[i];
				indices.push_back(x[i] - 1);
				textureIndices.push_back(y[i] - 1);
				nomalIndices.push_back(z[i] - 1);
				i++;
			}
		}

		else if (line.substr(0, 7) == "usemtl ")
		{
			if (indices.size() > 0)
			{
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}
		else if (line.substr(0, 7) == "mtllib " )
		{
			modelName = line.substr(7);
		}
	}
	PostProcessing();
	
}

void LoadOBJModel::LoadModel(const std::string & objFilePath, const std::string & matFilePath)
{
	LoadMaterialLibary(matFilePath);
	LoadModel(objFilePath);
}

std::vector<Vertex> LoadOBJModel::GetVers()
{
	return meshVerterices;
}

std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

BoundingBox LoadOBJModel::GetBoundingBox()
{
	return box;
}

void LoadOBJModel::OnDestroy()
{
		vertercies.clear();
		normals.clear();
		textureCoords.clear();
		indices.clear();
		nomalIndices.clear();
		textureIndices.clear();
		meshVerterices.clear();
		subMeshes.clear();
}

void LoadOBJModel::PostProcessing()
{
	for (int i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertercies[indices[i]];
		vert.normal = normals[nomalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVerterices.push_back(vert);
	}

	SubMesh subMesh;
	subMesh.vertexList = meshVerterices;
	subMesh.meshIndices = indices;
	subMesh.material = currentMaterial;

	subMeshes.push_back(subMesh);

	indices.clear();
	nomalIndices.clear();
	textureIndices.clear();
	meshVerterices.clear();
	currentMaterial = Material();

}

void LoadOBJModel::LoadMaterial(const string & fileName_)
{
	currentMaterial = MaterialHandler::getInstance()->GetMaterial(fileName_);
}

void LoadOBJModel::LoadMaterialLibary(const string & matFilePath_)
{
	MaterialLoader::loadMaterial(matFilePath_);
}
