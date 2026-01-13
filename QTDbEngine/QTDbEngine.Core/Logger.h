#pragma once
#include "pch.h"
#include "LogLevel.hpp"

class Logger {
public:
	Logger(const std::string& logFilePath);

public:
	void LogDebug(const std::string& message);
	void LogInfo(const std::string& message);
	void LogError(const std::string& message);
	void LogWarning(const std::string& message);

private:
	std::string m_logFilePath;
	/// <summary>
	/// The format for log messages: [Timestamp] [LogLevel]: Message
	/// </summary>
	std::string m_message_format = "[{current_time}] [{log_level}]: {message}";

	bool CheckFileExists();
	void CreateLogFile();
	void WriteToFile(const std::string& message);

	std::string FormatTemplate(const std::string& message, LogLevel logLevel);
	std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

	std::string GetCurrentTime();
};