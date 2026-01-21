#include <fstream>

#include "./DatabaseStorageLayer.h"
#include "../Utils/Paths.hpp"

DatabaseQueryStatusCode DatabaseStorageLayer::SaveTable(Table& table)
{
	std::string tableFilePath = GetPathForTable(table.DatabaseName, table.TableName);

	if (FileExists(tableFilePath))
	{
		return DatabaseQueryStatusCode::TABLE_ALREADY_EXISTS;
	}

	std::ofstream tableFileStream(tableFilePath);

	if (!tableFileStream.is_open()) {
		return DatabaseQueryStatusCode::FAILED;
	}

	tableFileStream << "Test";

	tableFileStream.close();
}