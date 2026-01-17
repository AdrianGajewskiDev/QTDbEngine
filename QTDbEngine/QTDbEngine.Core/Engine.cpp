#include "pch.h"
#include "Paths.hpp"
#include <iostream>
#include <filesystem>

constexpr const char* ENGINE_INITIALIZED_MESSAGE = "Engine initialized with version: ";

Engine::Engine(const std::string& version) : m_version(version) {}

void Engine::Initialize() {
	// Ensure paths
	EnsureDirectoriesExists();

	// Initialize logger
	m_logger = new Logger(GetFullLogPath());
	m_logger->LogInfo(ENGINE_INITIALIZED_MESSAGE + m_version);
}

DbCreationResult Engine::CreateNewDatabase(std::string& dbName)
{
	const std::string fullNewDbPath = std::filesystem::path(GetRootDatabasePath()).append(dbName).string();
	if (std::filesystem::exists(fullNewDbPath))
	{
		return DbCreationResult::FAILED_DB_NAME_TAKEN;
	}

	if (!std::filesystem::create_directories(fullNewDbPath))
	{
		return DbCreationResult::FAILED;
	}

	return DbCreationResult::CREATED;
}

std::vector<std::string> Engine::ListDatabases()
{
	std::vector<std::string> databases;

	for (auto file : std::filesystem::directory_iterator(GetRootDatabasePath()))
	{
		if (file.is_directory())
		{
			databases.push_back(file.path().filename().string());
		}
	}

	return databases;
}


Engine::~Engine() {
	delete m_logger;
}