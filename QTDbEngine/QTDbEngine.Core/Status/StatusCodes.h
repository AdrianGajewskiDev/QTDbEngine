#pragma once

enum class DatabaseQueryStatusCode {
	OK = 0,
	FAILED = 1,

	// Interpreter errors
	INVALID_QUERY = 2,

	// Resource Taken
	DATABASE_ALREADY_EXISTS,
	TABLE_ALREADY_EXISTS,

	// Resource Not exists
	DATABASE_NOT_EXISTS
};
