#include "ShaderHandler.h"

std::unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
std::map<std::string, GLuint>  ShaderHandler::programs = std::map<std::string, GLuint>();

ShaderHandler::ShaderHandler()
{

}
ShaderHandler::~ShaderHandler()
{
	if (programs.size() > 0)
	{
		OnDestroy();
	}
}

ShaderHandler * ShaderHandler::getInstance()
{
	if (shaderInstance.get() == nullptr)
	{
		shaderInstance.reset(new ShaderHandler);
	}
	return shaderInstance.get();
}

void ShaderHandler::CreateProgram(const std::string & shaderName_, const std::string & vertexShaderFilePath_, const std::string & fragmentShaderFilePath_)
{
	string vertexShaderCode = ReadShader(vertexShaderFilePath_);
	string fragmentShaderCode = ReadShader(fragmentShaderFilePath_);

	if (vertexShaderCode == "" || fragmentShaderCode == "")
	{
		return;
	}

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);

	if (vertexShader == 0 || fragmentShader == 0 )
	{
		return;
	}

	GLint linkResult = 0;
	GLint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	if (!linkResult)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> programLog(infoLogLength);
		glGetShaderInfoLog(program, infoLogLength, NULL, &programLog[0]);

		string programString(programLog.begin(), programLog.end());
		Debug::Error("Fail to link program " + shaderName_ + ". Error: " + programString, "ShaderHandler.cpp", __LINE__);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);


		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	programs[shaderName_] = program;

}

GLuint ShaderHandler::GetShader(const string & shaderName_)
{
	if (programs.find(shaderName_) != programs.end())
	{
		return programs[shaderName_];
	}

	return 0;
}

void ShaderHandler::OnDestroy()
{
	if (programs.size() > 0)
	{
		for (auto p : programs)
		{
			glDeleteProgram(p.second);
		}

	}
	programs.clear();

}


std::string ShaderHandler::ReadShader(const std::string & filePath_)
{
	std::string shaderCode;
	std::ifstream file;
	file.exceptions(ifstream::badbit);
	try
	{
		file.open(filePath_);
		std::stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();

		shaderCode = tmpStream.str();

	}
	catch (std::ifstream::failure error_)
	{
		Debug::Error("Could not read file" + filePath_, "ShaderHandler.cpp", __LINE__ );
		return "";
	}
	return shaderCode;
}

GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string & source_, const std::string & shaderName_)
{
	GLint compileResult = 0;

	GLuint shader = glCreateShader(shaderType_);
	const char* shaderCodePtr = source_.c_str();
	const int shaderCodeSize = source_.size();


	glShaderSource(shader, 1 , &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (!compileResult)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> shaderLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);

		string shaderString(shaderLog.begin(), shaderLog.end());
		Debug::Error("Fail to compile shader " + shaderName_ + "." + shaderString, "ShaderHandler.cpp", __LINE__);
		return 0;
	}
	return shader;
}
