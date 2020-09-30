#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <glew.h>
#include "../Core/Debug.h"

using namespace std;

class ShaderHandler
{
public:
	ShaderHandler(const ShaderHandler&) = delete;
	ShaderHandler(ShaderHandler&&) = delete;
	ShaderHandler& operator = (const ShaderHandler&) = delete;
	ShaderHandler& operator = (ShaderHandler&&) = delete;

	static ShaderHandler* getInstance();

	void CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFilePath_, 
					   const std::string& fragmentShaderFilePath_);
	GLuint GetShader(const string& shaderName_);

	void OnDestroy();


private:
	ShaderHandler();
	~ShaderHandler();
	static std::unique_ptr<ShaderHandler> shaderInstance;
	friend std::default_delete<ShaderHandler>;
	std::string ReadShader(const std::string& filePath_);
	GLuint CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_);
	
	static std::map<std::string, GLuint> programs;
};
#endif // !SHADERHANDLER_H


