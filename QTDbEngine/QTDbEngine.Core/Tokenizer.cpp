#include "pch.h"

const std::string KEYWORDS[7] = {
	"CREATE",
	"TABLE",
	"SELECT",
	"FROM",
	"INSERT",
	"INTO",
	"VALUES"
};

const char OPERATORS[4] = {
	'(',
	')',
	'='
};

std::expected<std::vector<Token>, TokenizerError> Tokenizer::Tokenize(std::string& rawSql)
{
	std::vector<Token> tokens;
	std::string substring;

	for (int i = 0; i < rawSql.size(); i++)
	{
		auto currChar = rawSql[i];

		if (currChar == '"') {
			std::expected<std::tuple<int, std::string>, TokenizerError> _stringToken = ReadString(rawSql, i);

			if (!_stringToken) {
				return std::unexpected(_stringToken.error());
			}

			tokens.push_back(Token{ .Type = SQLTokenType::STRING, .Value = std::get<1>(_stringToken.value()) });

			i += std::get<0>(_stringToken.value());
			continue;
		}

		if (std::isdigit(currChar) && substring.empty()) {
			std::expected<std::tuple<int, uint64_t>, TokenizerError> _stringToken = ReadNumber(rawSql, i);

			if (!_stringToken) {
				return std::unexpected(_stringToken.error());
			}

			tokens.push_back(Token{ .Type = SQLTokenType::NUMBER, .Value = std::get<1>(_stringToken.value()) });

			i += std::get<0>(_stringToken.value());
			continue;
		}
		
		if (currChar == '(') {
			tokens.push_back(Token{ .Type = SQLTokenType::OPERATOR, .Value = SQLOperator::OPEN_CURLY});
			continue;
		}

		if (currChar == ')') {
			if (!substring.empty()) {
				tokens.push_back(Token{ .Type = SQLTokenType::IDENTIFIER, .Value = substring });
				substring = "";
			}
			tokens.push_back(Token{ .Type = SQLTokenType::OPERATOR, .Value = SQLOperator::CLOSING_CURLY });
			continue;
		}

		if (currChar == ',') {
			if (!substring.empty()) {
				tokens.push_back(Token{ .Type = SQLTokenType::IDENTIFIER, .Value = substring });
				substring = "";
			}
			tokens.push_back(Token{ .Type = SQLTokenType::OPERATOR, .Value = SQLOperator::COMMA });
			continue;
		}

		if (currChar != ' ') {
			substring += currChar;
		}

		std::optional<std::string> keywordFound = CheckKeywords(substring);

		if (keywordFound) {
			tokens.push_back(Token{ .Type = SQLTokenType::KEYWORD, .Value = keywordFound.value()});
			substring = "";
			continue;
		}

		if ((currChar == ' ' || i + 1  == rawSql.size()) && !substring.empty()) {
			tokens.push_back(Token{ .Type = SQLTokenType::IDENTIFIER, .Value = substring });
			substring = "";
			continue;
		}
	}

	return tokens;
}

std::expected<std::tuple<int, std::string>, TokenizerError> Tokenizer::ReadString(std::string& rawSql, int currentIndex)
{
	std::string substring;
	currentIndex++;
	int steps = 0;
	while (currentIndex < rawSql.length()) {
		steps++;
		auto currChar = rawSql[currentIndex];
			
		if (currChar == '"') {
			return std::make_tuple(steps, substring);
		}

		substring += currChar;
		currentIndex++;
	}

	return std::unexpected(TokenizerError::EXPECTED_CHAR);
}

std::expected<std::tuple<int, uint64_t>, TokenizerError> Tokenizer::ReadNumber(std::string& rawSql, int currentIndex)
{
	std::string substring;
	substring += rawSql[currentIndex];
	currentIndex++;
	int steps = 0;
	while (std::isdigit(rawSql[currentIndex])) {
		steps++;
		auto currChar = rawSql[currentIndex];
		substring += currChar;
		currentIndex++;
	}

	return std::make_tuple(steps, std::stoul(substring));
}


std::optional<std::string> Tokenizer::CheckKeywords(std::string& substring)
{
	for (auto keyword : KEYWORDS) {
		if (substring == keyword)
		{
			return keyword;
		}
	}

	return std::nullopt;
}

std::optional<char> Tokenizer::CheckOperators(const char& currChar)
{
	for (auto keyword : OPERATORS) {
		if (currChar == keyword)
		{
			return keyword;
		}
	}

	return std::nullopt;
}