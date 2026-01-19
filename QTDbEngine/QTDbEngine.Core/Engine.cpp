#include "pch.h"
#include "Paths.hpp"
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

	m_tokenizer = new Tokenizer();
	m_parser = new Parser();
}

Engine::~Engine() {
	delete m_logger;
	delete m_tokenizer;
	delete m_parser;
}

DbCreationResult Engine::CreateNewDatabase(std::string& dbName)
{
	const std::string fullNewDbPath = std::filesystem::path(GetRootDatabasePath()).append(dbName).string();
	if (std::filesystem::exists(fullNewDbPath))
	{
		return DbCreationResult::FAILED_DB_NAME_TAKEN;
	}

	if (!std::filesystem::create_directories(fullNewDbPath))
	{
		return DbCreationResult::FAILED;
	}

	return DbCreationResult::CREATED;
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

QueryResult Engine::ExecuteRawSql(std::string& rawSql) 
{
	m_logger->LogDebug("Parsing sql query...");
	std::expected<std::vector<Token>, TokenizerError> tokens = m_tokenizer->Tokenize(rawSql);

	if (!tokens) {
		m_logger->LogWarning("Invalid Query");
		return QueryResult::TOKENIZER_ERROR;
	}

	m_logger->LogDebug("Query parsed successfully");

	std::expected<ParserResult, ParserError> parserResult = m_parser->ParseTokens(tokens.value());

	if (!parserResult) {
		m_logger->LogWarning("Failed to parse query");
		return QueryResult::PARSER_ERROR;
	}

	ProcessParserResult(parserResult.value());

	return QueryResult::OK;
}

void Engine::ProcessParserResult(ParserResult& parserResult)
{
	switch (parserResult.Command) {
		case DatabaseCommand::CREATE_DATABASE:
		{
			std::string dbName = std::get<std::string>(parserResult.Params.at("db_name"));
			m_logger->LogDebug("Creating new database: " + dbName);
			CreateNewDatabase(dbName);
		}; break;
		case DatabaseCommand::CREATE_TABLE:
		{
			std::string dbName = std::get<std::string>(parserResult.Params.at("db_name"));
			std::string tableName = std::get<std::string>(parserResult.Params.at("table_name"));
			std::vector<Column> columns = std::get<std::vector<Column>>(parserResult.Params.at("columns_definition"));
			m_logger->LogDebug("Creating new table: " + dbName);
			CreateNewTable(dbName, tableName, columns);
		}; break;
	}
}

DbCreationResult Engine::CreateNewTable(std::string& dbName, std::string tableName, std::vector<Column>& columns)
{
	return DbCreationResult::CREATED;
}