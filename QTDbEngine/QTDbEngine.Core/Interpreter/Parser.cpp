#include "Parser.h"
#include "../Utils/Utils.hpp"

std::expected<InterpreterResult, InterpreterError> Parser::ParseTokens(std::vector<Token>& inputTokens)
{
	const auto& firstClause = inputTokens[0];
	if (firstClause.Type != SQLTokenType::KEYWORD) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	SQLKeyword keyword = std::get<SQLKeyword>(firstClause.Value);
	switch (keyword) {
		case SQLKeyword::CREATE: return ProcessCreateClause(inputTokens); break;
		case SQLKeyword::SELECT: return ProcessSelectClause(inputTokens); break;
		default: return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}
}

std::expected<InterpreterResult, InterpreterError> Parser::ProcessCreateClause(std::vector<Token>& inputTokens)
{
	if (inputTokens.empty()) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	const auto& secondToken = inputTokens[1];

	if (secondToken.Type != SQLTokenType::KEYWORD) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	DatabaseCommand command;

	switch (std::get<SQLKeyword>(secondToken.Value)) {
		case SQLKeyword::DATABASE: command = DatabaseCommand::CREATE_DATABASE; break;
		case SQLKeyword::TABLE: command = DatabaseCommand::CREATE_TABLE; break;
		default: return std::unexpected(InterpreterError::FAILED_TO_PARSE); break;
	}

	std::expected<std::map<std::string, TokenValue>, InterpreterError> params = ParseCreateClauseParams(command, inputTokens);
	
	if (!params) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	return InterpreterResult{ .Command = command, .Params = params.value()};
}

std::expected<InterpreterResult, InterpreterError> Parser::ProcessSelectClause(std::vector<Token>& inputTokens)
{
	return std::unexpected(InterpreterError::FAILED_TO_PARSE);
}


std::expected<std::map<std::string, TokenValue>, InterpreterError> Parser::ParseCreateClauseParams(DatabaseCommand& command, std::vector<Token>& inputTokens)
{
	switch (command)
	{
		case DatabaseCommand::CREATE_DATABASE:
		{
			if (inputTokens.size() != 3)
			{
				return std::unexpected(InterpreterError::FAILED_TO_PARSE);
			}

			const auto& thirdToken = inputTokens[2];
			if (thirdToken.Type != SQLTokenType::IDENTIFIER) {
				return std::unexpected(InterpreterError::FAILED_TO_PARSE);
			}

			return std::map<std::string, TokenValue> {
				{"db_name", thirdToken.Value}
			};
		}; break;

		case DatabaseCommand::CREATE_TABLE:
		{
			// At least one column definition
			if (inputTokens.size() < 7)
			{
				return std::unexpected(InterpreterError::FAILED_TO_PARSE);
			}

			const auto& thirdToken = inputTokens[2];
			if (thirdToken.Type != SQLTokenType::IDENTIFIER) {
				return std::unexpected(InterpreterError::FAILED_TO_PARSE);
			}

			std::vector<std::string> dbAndTableName = Split(std::get<std::string>(thirdToken.Value), '.');

			if (dbAndTableName.size() != 2) {
				return std::unexpected(InterpreterError::FAILED_TO_PARSE);
			}

			std::string dbName = dbAndTableName[0];
			std::string tableName = dbAndTableName[1];

			std::expected<std::vector<Token>, InterpreterError> columnTokens = ExtractBetweenParenthesis(1, inputTokens);

			if (!columnTokens) {
				return std::unexpected(columnTokens.error());
			}

			std::vector<Column> columnDefinitions;
			for (int i = 0; i < columnTokens.value().size(); i++)
			{
				int step = 0;

				Token& currToken = columnTokens.value()[i];
				std::vector<Token> tempTokens;
				while (!IsVariantType(currToken.Value, SQLOperator::COMMA))
				{
					tempTokens.push_back(currToken);
					step++;
					int next = i + step;

					if (next > columnTokens.value().size() - 1 ) {
						break;
					}
					currToken = columnTokens.value()[i + step];
				}

				std::string columnName = std::get<std::string>(tempTokens[0].Value);
				SQLKeyword columnType = std::get<SQLKeyword>(tempTokens[1].Value);

				QTypes _cType = columnType == SQLKeyword::INT ? QTypes::INT64 : QTypes::STRING;

				columnDefinitions.push_back(Column {.Name = columnName, .Type = _cType});

				i += step;
			}

			return std::map<std::string, TokenValue> {
				{"db_name", dbName},
				{"table_name", tableName},
				{"columns_definition", columnDefinitions}
			};
		}; break;
		default: return std::unexpected(InterpreterError::FAILED_TO_PARSE); break;
	}
}

std::expected<std::vector<Token>, InterpreterError> Parser::ExtractBetweenParenthesis(int position, std::vector<Token>& inputTokens)
{
	std::ptrdiff_t firstOpenParenthesis = GetTokenPosition<SQLOperator>(1, SQLOperator::OPEN_CURLY, inputTokens);
	std::ptrdiff_t firstCloseParenthesis = GetTokenPosition<SQLOperator>(1, SQLOperator::CLOSING_CURLY, inputTokens);

	size_t openIdx = static_cast<size_t>(firstOpenParenthesis);
	size_t closeIdx = static_cast<size_t>(firstCloseParenthesis);

	if (openIdx >= inputTokens.size() || closeIdx > inputTokens.size() || openIdx >= closeIdx) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	return std::vector<Token>(inputTokens.begin() + openIdx + 1, inputTokens.begin() + closeIdx);
}