#include <string>
#include <filesystem>

constexpr const char* LOG_STORAGE_PATH = "./Logs";
constexpr const char* LOG_FILE_NAME = "QTDbEngine.log";

constexpr const char* DB_STORAGE_PATH = "./Database";
constexpr const char* DB_FILE_NAME = "QTDbEngine.db";

inline void EnsureDirectoriesExists() {
	const auto logPathExists = std::filesystem::exists(LOG_STORAGE_PATH) && std::filesystem::is_directory(LOG_STORAGE_PATH);
	const auto dbPathExists = std::filesystem::exists(DB_STORAGE_PATH) && std::filesystem::is_directory(DB_STORAGE_PATH);

	if (!logPathExists) {
		std::filesystem::create_directories(LOG_STORAGE_PATH);
	}

	if (!dbPathExists) {
		std::filesystem::create_directories(DB_STORAGE_PATH);
	}
}

inline std::string GetFullLogPath() {
	return std::filesystem::path(LOG_STORAGE_PATH).append(LOG_FILE_NAME).string();
}

inline std::string GetFullDatabasePath() {
	return std::filesystem::path(DB_STORAGE_PATH).append(DB_FILE_NAME).string();
}

inline std::string GetRootDatabasePath() {
	return std::filesystem::path(DB_STORAGE_PATH).string();
}

inline std::string GetPathForTable(std::string& dbName, std::string& tableName)
{
	auto basePath = std::filesystem::path(DB_STORAGE_PATH).append(dbName).string();

	return basePath + "/" + tableName + ".db";
}

inline bool FileExists(std::string& filePath) 
{
	return std::filesystem::is_regular_file(filePath);
}