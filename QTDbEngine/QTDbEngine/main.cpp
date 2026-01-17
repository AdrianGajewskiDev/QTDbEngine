#include "pch.h"
#include <iostream>
#include "CLIMenu.h"
#include <memory>

const std::string ENGINE_VERSION = "0.0.1";

CLIMenu* InitializeMenu(Engine& engine)
{
	MenuPage rootPage(std::vector<MenuEntry> {
			MenuEntry("1.List Databases", std::nullopt, std::nullopt),
				MenuEntry("2.Create New Database", [&engine]() {
					std::cout << "Enter database Name:";
					std::string dbName;
					std::cin >> dbName;

					DbCreationResult dbCreationResult = engine.CreateNewDatabase(dbName);

					if (dbCreationResult == DbCreationResult::CREATED) {
						std::cout << "Database have been created successfully." << std::endl;
						return;
					}

					if (dbCreationResult == DbCreationResult::FAILED) {
						std::cout << "Unknown Error have occured. Please try again." << std::endl;
						return;
					}

					if (dbCreationResult == DbCreationResult::FAILED_DB_NAME_TAKEN) {
						std::cout << "Database " << dbName << " already exists." << std::endl;
						return;
					}
				}, std::nullopt),
			MenuEntry("3.Execute SQL", std::nullopt, std::nullopt),
			MenuEntry("4.Exit", []() {
					exit(0);
				},
				std::nullopt
			)

	});

	CLIMenu cliMenu(rootPage);

	cliMenu.ShowMenu();

	return &cliMenu;
}

int main(int argc, char* argv[])
{
	Engine engine(ENGINE_VERSION);

	engine.Initialize();

	CLIMenu* cliMenu = InitializeMenu(engine);

	cliMenu->ShowMenu();
}