#pragma once
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <expected>
#include "../Data/Column.h"
#include "Tokenizer.h"


class Parser {
public:
	std::expected<InterpreterResult, InterpreterError> ParseTokens(std::vector<Token>& inputTokens);

private:
	std::expected<InterpreterResult, InterpreterError> ProcessCreateClause(std::vector<Token>& inputTokens);
	std::expected<InterpreterResult, InterpreterError> ProcessSelectClause(std::vector<Token>& inputTokens);

	std::expected<std::map<std::string, std::variant<TokenValue, std::vector<TokenValue>>>, InterpreterError> ParseCreateClauseParams(DatabaseCommand& command, std::vector<Token>& inputTokens);

	std::expected<std::vector<Token>, InterpreterError> ExtractBetweenParenthesis(int position, std::vector<Token>& inputTokens);
	std::expected<std::vector<Token>, InterpreterError> ExtractBetweenKeywords(SQLKeyword from, SQLKeyword to, std::vector<Token>& inputTokens);
};