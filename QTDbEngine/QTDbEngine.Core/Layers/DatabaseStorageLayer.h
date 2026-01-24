#pragma once
#include <memory>
#include "../Binary/Writer/BinaryWriter.h"
#include "../Data/Column.h"
#include "../Status/StatusCodes.h"

class DatabaseStorageLayer {
public:
	DatabaseStorageLayer() : m_binaryWriter(std::make_unique<BinaryWriter>()) {}
protected:
	DatabaseQueryStatusCode SaveTable(Table& table);

private:
	std::unique_ptr<BinaryWriter> m_binaryWriter;
};