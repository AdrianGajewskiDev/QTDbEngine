#pragma once
#include <vector>
#include <optional>
#include <expected>
#include <variant>
#include <string>
#include <tuple>
#include "../Data/Column.h"
#include "Models/Models.h"


struct Token {
public:
	SQLTokenType Type;
	TokenValue Value;
};

class Tokenizer {
public:
	std::expected<std::vector<Token>, TokenizerError> Tokenize(std::string& rawSql);
private:
	std::expected<std::tuple<int, std::string>, TokenizerError> ReadString(std::string& rawSql, int currentIndex);
	std::expected<std::tuple<int, uint64_t>, TokenizerError> ReadNumber(std::string& rawSql, int currentIndex);

	std::optional<SQLKeyword> CheckKeywords(std::string& substring);
	std::optional<char> CheckOperators(const char& currChar);
};