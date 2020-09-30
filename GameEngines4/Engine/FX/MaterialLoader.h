#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include "../Graphics/MaterialHandler.h"
#include "../Graphics/TextureHandler.h"
#include <sstream>

class MaterialLoader
{
public:
	MaterialLoader(const MaterialLoader&) = delete;
	MaterialLoader(MaterialLoader&&) = delete;
	MaterialLoader& operator = (const MaterialLoader&) = delete;
	MaterialLoader& operator = (MaterialLoader&&) = delete;

	MaterialLoader() = delete;

	static void loadMaterial(string filePath_ );

	~MaterialLoader();
private:
	static GLuint LoadTexture(string fileName_);
};

#endif // MATERIALLOADER_H

