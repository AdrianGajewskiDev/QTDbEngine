#include "pch.h"
#include "Paths.hpp"
#include <iostream>

constexpr const char* ENGINE_INITIALIZED_MESSAGE = "Engine initialized with version: ";

Engine::Engine(const std::string& version) : m_version(version) {}

void Engine::Initialize() {
	// Ensure paths
	EnsureDirectoriesExists();

	// Initialize logger
	m_logger = new Logger(GetFullLogPath());
	m_logger->LogInfo(ENGINE_INITIALIZED_MESSAGE + m_version);
}

Engine::~Engine() {
	delete m_logger;
}