#pragma once
#include <string>
#include <map>
#include "Datatypes.hpp"
#include <vector>

struct Column {
public:
	std::string Name;
	QTypes Type;
	std::map<std::string, std::string> Attributes;
};


struct Table {
public:
	std::string DatabaseName;
	const char* TableName;
	std::vector<Column> Columns;
};
