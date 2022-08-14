#pragma once
#include <chrono>
#include <string>
#include <fstream> 

class Logger
{
private: 
	enum class Severity { 
		Debug, Error, Stream
	};
public:
	Logger();
	~Logger();	
	void log_error(const std::string& text) { Log(Severity::Debug, text); }
	void log_debug(const std::string& text) { Log(Severity::Debug, text); }
	void log_stream(const std::string& text);

private:
	void Log(Severity, std::string message);
	std::ofstream logStream_; 
	std::ofstream fileStream_;
};

