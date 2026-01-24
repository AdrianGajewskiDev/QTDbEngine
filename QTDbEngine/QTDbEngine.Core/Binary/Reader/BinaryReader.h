#pragma once
#include <fstream>
#include "../Layout.h"

class BinaryReader {
public:
	BinaryHeader ReadHeader(std::ifstream& stream);
};
