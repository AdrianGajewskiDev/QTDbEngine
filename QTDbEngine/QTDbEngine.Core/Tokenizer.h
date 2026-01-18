#pragma once
#include <vector>
#include <optional>
#include <expected>
#include <variant>
#include <string>
#include <tuple>

enum class SQLTokenType {
	KEYWORD		= 0,
	IDENTIFIER  = 1,
	STRING		= 2,
	NUMBER		= 3,
	OPERATOR    = 4
};

enum class SQLKeyword {
	CREATE	 = 0,
	TABLE	 = 1,
	SELECT	 = 2,
	FROM	 = 4,
	INSERT	 = 5,
	INTO	 = 6,
	VALUES	 = 9,
};

enum class SQLOperator {
	EQUAL			= 0,
	OPEN_CURLY		= 1,
	CLOSING_CURLY	= 2,
	COMMA			= 3
};

enum class TokenizerError {
	UNEXPECTED_CHAR = 0,
	EXPECTED_CHAR = 1
};


struct Token {
public:
	SQLTokenType Type;
	std::variant<std::string, SQLKeyword, SQLOperator, uint64_t, char> Value;
};

class Tokenizer {
public:
	std::expected<std::vector<Token>, TokenizerError> Tokenize(std::string& rawSql);
private:
	std::expected<std::tuple<int, std::string>, TokenizerError> ReadString(std::string& rawSql, int currentIndex);
	std::expected<std::tuple<int, uint64_t>, TokenizerError> ReadNumber(std::string& rawSql, int currentIndex);

	std::optional<std::string> CheckKeywords(std::string& substring);
	std::optional<char> CheckOperators(const char& currChar);
};