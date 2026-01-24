#pragma once
#include <cstdint>

struct BinaryHeader {
	char magic[4];
	uint16_t version;
};
