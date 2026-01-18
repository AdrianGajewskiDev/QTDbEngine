#pragma once
#include <vector>
#include "Logger.h"
#include "Tokenizer.h"

enum class DbCreationResult {
	CREATED = 0,
	FAILED_DB_NAME_TAKEN,
	FAILED
};

enum class QueryResult {
	OK = 0,
	TOKENIZER_ERROR = 1
};

class Engine {
public:
	Engine(const std::string& version);
	~Engine();
public:
	void Initialize();
	DbCreationResult CreateNewDatabase(std::string& dbName);
	QueryResult ExecuteRawSql(std::string& rawSql);
	std::vector<std::string> ListDatabases();
private:
	std::string m_version;
	Logger* m_logger;
	Tokenizer* m_tokenizer;
};