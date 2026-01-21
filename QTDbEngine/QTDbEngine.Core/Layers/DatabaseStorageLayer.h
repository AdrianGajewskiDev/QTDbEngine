#pragma once
#include "../Data/Column.h"
#include "../Status/StatusCodes.h"

class DatabaseStorageLayer {
protected:
	DatabaseQueryStatusCode SaveTable(Table& table);
};