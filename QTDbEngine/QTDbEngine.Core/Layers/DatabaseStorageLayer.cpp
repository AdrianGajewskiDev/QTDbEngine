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

	std::ofstream tableFileStream(tableFilePath, std::ios::binary);

	if (!tableFileStream.is_open()) {
		return DatabaseQueryStatusCode::FAILED;
	}

	m_binaryWriter->WriteTableHeader(tableFileStream, table.TableName);

	tableFileStream.close();
}