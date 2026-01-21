#pragma once
#include <expected>
#include <string>
#include "../Interpreter/Tokenizer.h"
#include "../Interpreter/Parser.h"
#include "Models/Models.h"

class Interpreter {
public:
	Interpreter();
	~Interpreter();

public:
	std::expected<InterpreterResult, InterpreterError> InterpretQuery(std::string& sqlQuery);
private:
	Tokenizer* m_tokenizer;
	Parser* m_parser;
};