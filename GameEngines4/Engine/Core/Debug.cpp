#include "Debug.h"

MessageType Debug::currentSev = MessageType::TYPE_NONE;
std::string Debug::outputName = "";


void Debug::DebugInit()
{
	outputName = "GameEngine_Log.txt";
	std::ofstream out;
	out.open(outputName.c_str(), std::ios::out);
	out.close();

	currentSev = MessageType::TYPE_FATAL_ERROR;
}

void Debug::SetSeverity(MessageType type_)
{
	currentSev = type_;
}

void Debug::Info(const std::string & message_, const std::string & filename_, const int line_)
{
	Log(MessageType::TYPE_INFO, "[INFO]:"+ message_, filename_, line_);
}

void Debug::Trace(const std::string & message_, const std::string & filename_, const int line_)
{
	Log(MessageType::TYPE_TRACE, "[TRACE]:" + message_, filename_, line_);
}

void Debug::Warning(const std::string & message_, const std::string & filename_, const int line_)
{
	Log(MessageType::TYPE_WARNING, "[WARNING]:" + message_, filename_, line_);
}

void Debug::Error(const std::string & message_, const std::string & filename_, const int line_)
{
	Log(MessageType::TYPE_ERROR, "[ERROR]:" + message_, filename_, line_);
}

void Debug::Fatal_error(const std::string & message_, const std::string & filename_, const int line_)
{
	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL_ERROR]:" + message_, filename_, line_);
}

void Debug::Log(MessageType type_, const std::string & message_, const std::string & filename_, const int line_)
{
	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE)
	{
		std::ofstream out;
		out.open(outputName.c_str(), std::ios::app | std::ios::out);
		out << message_ << " in: " << filename_ << " occur line: " << line_ << std::endl;
		out.flush();
		out.close();
	}
}


Debug::~Debug()
{
}
