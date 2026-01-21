#include "Engine.h"
#include "../Utils/Paths.hpp"
#include <iostream>
#include <filesystem>

constexpr const char* ENGINE_INITIALIZED_MESSAGE = "Engine initialized with version: ";

Engine::Engine(const std::string& version) : m_version(version) {}

void Engine::Initialize() {
	// Ensure paths
	EnsureDirectoriesExists();

	// Initialize logger
	m_logger = new Logger(GetFullLogPath());
	m_logger->LogInfo(ENGINE_INITIALIZED_MESSAGE + m_version);

	m_interpreter = new Interpreter();

}

Engine::~Engine() {
	delete m_logger;
	delete m_interpreter;
}

DatabaseQueryStatusCode Engine::CreateNewDatabase(std::string& dbName)
{
	const std::string fullNewDbPath = std::filesystem::path(GetRootDatabasePath()).append(dbName).string();
	if (std::filesystem::exists(fullNewDbPath))
	{
		return DatabaseQueryStatusCode::DATABASE_ALREADY_EXISTS;
	}

	if (!std::filesystem::create_directories(fullNewDbPath))
	{
		return DatabaseQueryStatusCode::FAILED;
	}

	return DatabaseQueryStatusCode::OK;
}

std::vector<std::string> Engine::ListDatabases()
{
	std::vector<std::string> databases;

	for (auto file : std::filesystem::directory_iterator(GetRootDatabasePath()))
	{
		if (file.is_directory())
		{
			databases.push_back(file.path().filename().string());
		}
	}

	return databases;
}

DatabaseQueryStatusCode Engine::ExecuteRawSql(std::string& rawSql)
{
	auto interpreterResult = m_interpreter->InterpretQuery(rawSql);
	if (!interpreterResult) {
		m_logger->LogWarning("Failed to parse query");
		return DatabaseQueryStatusCode::INVALID_QUERY;
	}

	return ProcessParserResult(interpreterResult.value());
}

DatabaseQueryStatusCode Engine::ProcessParserResult(InterpreterResult& parserResult)
{
	switch (parserResult.Command) {
		case DatabaseCommand::CREATE_DATABASE:
		{
			std::string dbName = std::get<std::string>(parserResult.Params.at("db_name"));
			m_logger->LogDebug("Creating new database: " + dbName);
			return CreateNewDatabase(dbName);
		}; break;
		case DatabaseCommand::CREATE_TABLE:
		{
			std::string dbName = std::get<std::string>(parserResult.Params.at("db_name"));
			std::string tableName = std::get<std::string>(parserResult.Params.at("table_name"));
			std::vector<Column> columns = std::get<std::vector<Column>>(parserResult.Params.at("columns_definition"));
			m_logger->LogDebug("Creating new table: " + dbName);
			return CreateNewTable(dbName, tableName, columns);
		}; break;
	}
}

DatabaseQueryStatusCode Engine::CreateNewTable(std::string& dbName, std::string tableName, std::vector<Column>& columns)
{
	const std::vector<std::string> databases = ListDatabases();

	if (std::find(databases.begin(), databases.end(), dbName) == databases.end())
	{
		return DatabaseQueryStatusCode::DATABASE_NOT_EXISTS;
	}

	Table table = { .DatabaseName = dbName, .TableName = tableName, .Columns = columns };

	return SaveTable(table);
}