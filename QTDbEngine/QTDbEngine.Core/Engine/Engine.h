#pragma once
#include <vector>
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
	DatabaseQueryStatusCode CreateNewDatabase(std::string& dbName);
	DatabaseQueryStatusCode CreateNewTable(std::string& dbName, std::string tableName, std::vector<Column>& columns);
	DatabaseQueryStatusCode ExecuteRawSql(std::string& rawSql);
	std::vector<std::string> ListDatabases();
private:
	std::string m_version;
	Logger* m_logger;
	Interpreter* m_interpreter;
private:
	DatabaseQueryStatusCode ProcessParserResult(InterpreterResult& parserResult);
};