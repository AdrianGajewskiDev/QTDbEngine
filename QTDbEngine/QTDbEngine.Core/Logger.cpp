#include "pch.h"
#include <fstream>
#include <format>

# define LOG_LOADED_MESSAGE "Loaded existing log file from storage"
# define LOG_CREATED_MESSAGE "Log file created."

Logger::Logger(const std::string& logFilePath) : m_logFilePath(logFilePath) {
	if (!CheckFileExists()) {
		CreateLogFile();
	}
	else {
		LogDebug(LOG_LOADED_MESSAGE);
	}
}

bool Logger::CheckFileExists() {
	std::ifstream file(m_logFilePath);
	return file.good();
}

void Logger::CreateLogFile() {
	std::ofstream file(m_logFilePath);
	std::string messageFormatted = FormatTemplate(LOG_CREATED_MESSAGE, LogLevel::Info);
	file << messageFormatted << std::endl;
	file.close();
}

void Logger::WriteToFile(const std::string& message) {
	std::ofstream file(m_logFilePath, std::ios::app);
	file << message << std::endl;
	file.close();
}

std::string Logger::GetCurrentTime() {
	std::time_t now = std::time(nullptr);
	char buf[32];
	std::tm tm;
#if defined(_MSC_VER)
	// Use thread-safe localtime_s on MSVC
	localtime_s(&tm, &now);
#else
	// Use thread-safe localtime_r on POSIX
	localtime_r(&now, &tm);
#endif
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
	return std::string(buf);
}



std::string Logger::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
	return str;
}

std::string Logger::FormatTemplate(const std::string& message, LogLevel logLevel) {
	std::string formatted = m_message_format;
	formatted = ReplaceAll(formatted, "{current_time}", GetCurrentTime());
	formatted = ReplaceAll(formatted, "{log_level}", LogLevelToString(logLevel));
	formatted = ReplaceAll(formatted, "{message}", message);
	return formatted;
}

void Logger::LogInfo(const std::string& message) {
	std::string formattedMessage = FormatTemplate(message, LogLevel::Info);
	WriteToFile(formattedMessage);
	std::printf("%s\n", formattedMessage.c_str());
}

void Logger::LogError(const std::string& message) {
	std::string formattedMessage = FormatTemplate(message, LogLevel::Error);
	WriteToFile(formattedMessage);
	std::fprintf(stderr, "%s\n", formattedMessage.c_str());
}

void Logger::LogWarning(const std::string& message) {
	std::string formattedMessage = FormatTemplate(message, LogLevel::Warning);
	WriteToFile(formattedMessage);
	std::printf("%s\n", formattedMessage.c_str());
}

void Logger::LogDebug(const std::string& message) {
	std::string formattedMessage = FormatTemplate(message, LogLevel::Debug);
	WriteToFile(formattedMessage);
	std::printf("%s\n", formattedMessage.c_str());
}