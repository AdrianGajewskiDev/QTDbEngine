#pragma once
#include <vector>
#include <tuple>
#include "../Logger/Logger.h"
#include "../Interpreter/Tokenizer.h"
#include "../Interpreter/Parser.h"
#include "../Data/Column.h"
#include "../Layers/DatabaseStorageLayer.h"
#include "../Status/StatusCodes.h"
#include "../Interpreter/Interpreter.h"


class Engine : public DatabaseStorageLayer {
public:
	Engine(const std::string& version);
	~Engine();
public:
	void Initialize();
	std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> CreateNewDatabase(std::string& dbName);
	std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> CreateNewTable(std::string& dbName, const char* tableName, std::vector<Column>& columns);
	std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> ExecuteRawSql(std::string& rawSql);
	std::vector<std::string> ListDatabases();
private:
	std::string m_version;
	Logger* m_logger;
	Interpreter* m_interpreter;
private:
	std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> ProcessParserResult(InterpreterResult& parserResult);
};