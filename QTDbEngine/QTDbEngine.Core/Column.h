#pragma once
#include <string>
#include <map>
#include "Datatypes.hpp"

struct Column {
public:
	std::string Name;
	QTypes Type;
	std::map<std::string, std::string> Attributes;
};
