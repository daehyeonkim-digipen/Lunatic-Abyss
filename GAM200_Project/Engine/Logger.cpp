#include "Logger.h"
#include <iostream>

Logger::Logger() {
	fileStream_.open("LogOutput.log");
#ifdef _DEBUG
	logStream_.set_rdbuf(std::cout.rdbuf());
#else
	logStream_.open("LogRelease.log");
#endif // DEBUG

}

Logger::~Logger() {
	fileStream_.close();
	logStream_.close();
}

void Logger::log_stream(const std::string& text) {
	fileStream_ << text << '\n';
}

void Logger::Log(Severity currentLevel, std::string message)
{
	std::string information_Type;
	switch (currentLevel)
	{
	case Severity::Debug:
		information_Type = "Debug | ";
		break;
	case Severity::Error:
		throw std::runtime_error("Error | " + message );
		break;
	}

	logStream_ << information_Type << message << '\n';
}