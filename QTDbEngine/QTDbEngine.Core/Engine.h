#pragma once
#include "Logger.h"
#include <vector>

enum class DbCreationResult {
	CREATED = 0,
	FAILED_DB_NAME_TAKEN,
	FAILED
};


class Engine {
public:
	Engine(const std::string& version);
	~Engine();
public:
	void Initialize();
	DbCreationResult CreateNewDatabase(std::string& dbName);
	std::vector<std::string> ListDatabases();
private:
	std::string m_version;
	Logger* m_logger;
};