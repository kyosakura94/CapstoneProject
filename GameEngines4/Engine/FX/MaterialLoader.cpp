#include "MaterialLoader.h"

MaterialLoader::~MaterialLoader()
{
}

void MaterialLoader::loadMaterial(string filePath_)
{
	std::ifstream in(filePath_.c_str(), ios::in);
	if (!in)
	{
		Debug::Error("Cannot open MTL file:" + filePath_, "LoadOBJModel.cpp", __LINE__);
		return;
	}
	string line;
	Material m = Material();
	string matName = "";
	while (getline(in, line))
	{
		if (line.substr(0, 7) == "newmtl ")
		{
			if (m.diffuseMap != 0)
			{
				MaterialHandler::getInstance()->AddMaterial(m);
				m = Material();
			}
			matName = line.substr(7);
			m.diffuseMap = LoadTexture(matName);
			m.name = matName;
		}
		else if (line.substr(0, 4) == "\tNs ")
		{
			istringstream v(line.substr(4));
			float x;
			v >> x; 
			m.shininess = x;
		}
		else if (line.substr(0, 3) == "\td ")
		{
			istringstream v(line.substr(3));
			float x;
			v >> x; 
			m.transparency = x;
		}
		else if (line.substr(0, 4) == "\tKa ")
		{
			istringstream v(line.substr(4));
			float x, y, z;
			v >> x >> y >> z;
			m.ambient = vec3(x,y,z);
		}
		else if (line.substr(0, 4) == "\tKd ")
		{
			istringstream v(line.substr(4));
			float x, y, z;
			v >> x >> y >> z;
			m.diffuse = vec3(x, y, z);
		}
		else if (line.substr(0, 4) == "\tKs ")
		{
			istringstream v(line.substr(4));
			float x, y, z;
			v >> x >> y >> z;
			m.spectular = vec3(x, y, z);
		}

		//TODO: FISNISH loading the revalent information
	}
	if (m.diffuseMap != 0)
	{
		MaterialHandler::getInstance()->AddMaterial(m);
		m = Material();
	}
	in.close();
}

GLuint MaterialLoader::LoadTexture(string fileName_)
{
	GLuint currentTexture = TextureHandler::getInstance()->GetTexture(fileName_);
	if (currentTexture == 0)
	{
		TextureHandler::getInstance()->CreateTexture(fileName_,
			"./Resources/Textures/" + fileName_ + ".jpg");
		currentTexture = TextureHandler::getInstance()->GetTexture(fileName_);
	}
	return currentTexture;
}
