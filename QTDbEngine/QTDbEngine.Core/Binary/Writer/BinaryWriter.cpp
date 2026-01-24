#include "BinaryWriter.h"
#include "../Version.h"
#include "../Converter.h"

void BinaryWriter::WriteTableHeader(std::ofstream& stream, const char *tableName)
{
	uint16_t version = static_cast<uint16_t>(VERSION);
	
	const char* magic = "MYDB";

	WriteString(stream, magic, true);

	stream.write(BinaryConverter::IntToRawBinary<uint16_t>(version), sizeof(uint16_t)); // 4 bytes + 
}

void BinaryWriter::WriteString(std::ofstream& stream, const char* value, bool writeLength)
{
	uint16_t length = static_cast<uint16_t>(strlen(value));
	if (writeLength) {
		stream.write(BinaryConverter::IntToRawBinary<uint16_t>(length), sizeof(uint16_t));
	}

	stream.write(value, length);
}