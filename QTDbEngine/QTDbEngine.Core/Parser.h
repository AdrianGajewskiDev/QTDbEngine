#pragma once
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <expected>
#include "Column.h"
#include "pch.h"

enum class DatabaseCommand {
	CREATE_DATABASE = 0,
	CREATE_TABLE	= 1,

	SELECT			= 3,

	INSERT			= 4
};

enum class ParserError {
	FAILED_TO_PARSE
};

struct ParserResult {
	DatabaseCommand Command;
	std::map <std::string, TokenValue> Params;
};

class Parser {
public:
	std::expected<ParserResult, ParserError> ParseTokens(std::vector<Token>& inputTokens);

private:
	std::expected<ParserResult, ParserError> ProcessCreateClause(std::vector<Token>& inputTokens);
	std::expected<ParserResult, ParserError> ProcessSelectClause(std::vector<Token>& inputTokens);

	std::expected<std::map<std::string, TokenValue>, ParserError> ParseCreateClauseParams(DatabaseCommand& command, std::vector<Token>& inputTokens);

	std::expected<std::vector<Token>, ParserError> ExtractBetweenParenthesis(int position, std::vector<Token>& inputTokens);
};