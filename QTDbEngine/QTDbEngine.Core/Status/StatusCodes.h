#pragma once

enum class DatabaseQueryStatusCode {
	OK = 0,
	FAILED = 1,

	// Interpreter errors
	INVALID_QUERY = 2,

	// Resource Taken
	DATABASE_ALREADY_EXISTS = 3,
	TABLE_ALREADY_EXISTS = 4,

	// Resource Not exists
	DATABASE_NOT_EXISTS = 5
};