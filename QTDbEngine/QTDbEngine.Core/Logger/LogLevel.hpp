#pragma once
#include <string>

enum class LogLevel {
	Debug,
	Info,
	Warning,
	Error,
	Critical,
};

inline std::string LogLevelToString(LogLevel level) {
	switch (level) {
	case LogLevel::Debug:
		return "DEBUG";
	case LogLevel::Info:
		return "INFO";
	case LogLevel::Warning:
		return "WARNING";
	case LogLevel::Error:
		return "ERROR";
	case LogLevel::Critical:
		return "CRITICAL";
	default:
		return "UNKNOWN";
	}
}