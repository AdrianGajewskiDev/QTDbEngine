#pragma once
#include <string>
#include <map>
#include <variant>

enum class SQLTokenType {
	KEYWORD = 0,
	IDENTIFIER = 1,
	STRING = 2,
	NUMBER = 3,
	OPERATOR = 4
};

enum class SQLKeyword {
	CREATE = 0,
	TABLE = 1,
	SELECT = 2,
	FROM = 4,
	INSERT = 5,
	INTO = 6,
	VALUES = 7,
	DATABASE = 8,
	INT = 9,
	VARCHAR = 10,
	PRIMARY = 11,
	KEY = 12
};

enum class SQLOperator {
	EQUAL = 0,
	OPEN_CURLY = 1,
	CLOSING_CURLY = 2,
	COMMA = 3
};

enum class TokenizerError {
	UNEXPECTED_CHAR = 0,
	EXPECTED_CHAR = 1
};

using TokenValue = std::variant<std::string, SQLKeyword, SQLOperator, uint64_t, char, std::vector<Column>>;

enum class DatabaseCommand {
	CREATE_DATABASE = 0,
	CREATE_TABLE = 1,

	SELECT = 3,

	INSERT = 4
};

enum class InterpreterError {
	FAILED_TO_PARSE
};

struct InterpreterResult {
	DatabaseCommand Command;
	std::map <std::string, TokenValue> Params;
};

