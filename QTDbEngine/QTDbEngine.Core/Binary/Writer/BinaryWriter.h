#pragma once
#include <fstream>

class BinaryWriter {
public:
	void WriteTableHeader(std::ofstream& stream, const char *tableName);
	void WriteString(std::ofstream& stream, const char* tableName, bool writeLength);
};