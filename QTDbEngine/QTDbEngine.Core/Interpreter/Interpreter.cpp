#include "Interpreter.h"
#include "../Status/StatusCodes.h"

Interpreter::Interpreter() : m_parser(new Parser()), m_tokenizer(new Tokenizer())
{}

Interpreter::~Interpreter() {
	delete m_tokenizer;
	delete m_parser;
}

std::expected<InterpreterResult, InterpreterError> Interpreter::InterpretQuery(std::string& sqlQuery)
{
	std::expected<std::vector<Token>, TokenizerError> tokens = m_tokenizer->Tokenize(sqlQuery);

	if (!tokens) {
		return std::unexpected(InterpreterError::FAILED_TO_PARSE);
	}

	return m_parser->ParseTokens(tokens.value());
}