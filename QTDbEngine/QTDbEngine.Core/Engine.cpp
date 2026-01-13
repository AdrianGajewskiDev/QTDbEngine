#include "pch.h"
#include "Paths.hpp"
#include <iostream>

Engine::Engine(const std::string& version) : m_version(version) {}

void Engine::Initialize() {
	// Ensure paths
	EnsureDirectoriesExists();

	// Initialize logger
	m_logger = new Logger(GetFullLogPath());
	m_logger->LogInfo("Engine version " + m_version + " initialized.");
}

Engine::~Engine() {
	delete m_logger;
}