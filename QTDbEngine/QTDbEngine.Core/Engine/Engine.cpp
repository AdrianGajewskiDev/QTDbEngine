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

std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> Engine::CreateNewDatabase(std::string& dbName)
{
	const std::string fullNewDbPath = std::filesystem::path(GetRootDatabasePath()).append(dbName).string();
	if (std::filesystem::exists(fullNewDbPath))
	{
		return { DatabaseQueryStatusCode::DATABASE_ALREADY_EXISTS, std::nullopt };
	}

	if (!std::filesystem::create_directories(fullNewDbPath))
	{
		return { DatabaseQueryStatusCode::FAILED, std::nullopt };
	}

	return { DatabaseQueryStatusCode::OK, std::nullopt };
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

std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> Engine::ExecuteRawSql(std::string& rawSql)
{
	auto interpreterResult = m_interpreter->InterpretQuery(rawSql);
	if (!interpreterResult) {
		m_logger->LogWarning("Failed to parse query");
		return { DatabaseQueryStatusCode::INVALID_QUERY, std::nullopt };
	}

	return ProcessParserResult(interpreterResult.value());
}

std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> Engine::ProcessParserResult(InterpreterResult& parserResult)
{
	switch (parserResult.Command) {
		case DatabaseCommand::CREATE_DATABASE:
		{
			std::string dbName = parserResult.GetParams<std::string>("db_name").value();
			m_logger->LogDebug("Creating new database: " + dbName);
			return CreateNewDatabase(dbName);
		}; break;
		case DatabaseCommand::CREATE_TABLE:
		{
			std::string dbName = parserResult.GetParams<std::string>("db_name").value();
			std::string tableName = parserResult.GetParams<std::string>("table_name").value();
			std::vector<Column> columns = parserResult.GetParams<std::vector<Column>>("columns_definition").value();
			m_logger->LogDebug("Creating new table: " + dbName);
			return CreateNewTable(dbName, tableName.c_str(), columns);
		}; break;
		case DatabaseCommand::SELECT:
		{

		}; break;
	}
}

std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> Engine::CreateNewTable(std::string& dbName, const char* tableName, std::vector<Column>& columns)
{
	const std::vector<std::string> databases = ListDatabases();

	if (std::find(databases.begin(), databases.end(), dbName) == databases.end())
	{
		return { DatabaseQueryStatusCode::DATABASE_NOT_EXISTS, std::nullopt };
	}

	Table table = { .DatabaseName = dbName, .TableName = tableName, .Columns = columns };

	return { SaveTable(table), std::nullopt };
}