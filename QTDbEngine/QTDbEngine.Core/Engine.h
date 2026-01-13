#pragma once
#include "Logger.h"

class Engine {
public:
	Engine(const std::string& version);
	~Engine();
public:
	void Initialize();
private:
	std::string m_version;
	Logger* m_logger;
};