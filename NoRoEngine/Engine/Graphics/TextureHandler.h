#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <memory>

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>

#include "../Core/Debug.h"

using namespace std;

struct Texture {
	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
	std::string textureName = "";
};

class TextureHandler
{
public:
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator = (const TextureHandler&) = delete;
	TextureHandler& operator = (TextureHandler&&) = delete;

	static TextureHandler* getInstance();

	void OnDestroy();
	void CreateTexture(const string& textureName_, const string& textureFilePath_);
	static const GLuint GetTexture(const string& textureName_);
	static const Texture* GetTextureData(const string& textureName_);

private:

	TextureHandler();
	~TextureHandler();

	static unique_ptr<TextureHandler> textureInstance;
	friend default_delete<TextureHandler>;
	static vector<Texture*> textures;


};
#endif // !TEXTUREHANDLER_H


