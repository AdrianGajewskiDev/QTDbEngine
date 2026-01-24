#include "./Engine/Engine.h"
#include <iostream>
#include "CLIMenu.h"
#include <memory>
#include "./Status/StatusCodes.h"

const std::string ENGINE_VERSION = "0.0.1";

CLIMenu InitializeMenu(Engine& engine)
{
	MenuPage rootPage(std::vector<MenuEntry> {
		MenuEntry("1.List Databases", [&engine]() {
			const std::vector<std::string> databases = engine.ListDatabases();

			if (databases.empty()) {
				std::cout << "No Databases created yet." << std::endl;
				return;
			}

			for (std::string db : databases)
			{
				std::cout << "-- " << db << std::endl;
			}
		}, std::nullopt),
		MenuEntry("2.Create New Database", [&engine]() {
			std::cout << "Enter database Name:";
			std::string dbName;
			std::cin >> dbName;

			std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> dbCreationResult = engine.CreateNewDatabase(dbName);

			if (std::get<0>(dbCreationResult) == DatabaseQueryStatusCode::OK) {
				std::cout << "Database have been created successfully." << std::endl;
				return;
			}

			if (std::get<0>(dbCreationResult) == DatabaseQueryStatusCode::FAILED) {
				std::cout << "Unknown Error have occured. Please try again." << std::endl;
				return;
			}

			if (std::get<0>(dbCreationResult) == DatabaseQueryStatusCode::DATABASE_ALREADY_EXISTS) {
				std::cout << "Database " << dbName << " already exists." << std::endl;
				return;
			}
		}, std::nullopt),
		MenuEntry("3.Execute SQL", [&engine]() {
			std::string sql;
			std::cout << "Enter your sql here: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
			std::getline(std::cin, sql);
			std::tuple<DatabaseQueryStatusCode, std::optional<std::string>> queryResult = engine.ExecuteRawSql(sql);

			if (std::get<0>(queryResult) == DatabaseQueryStatusCode::OK) {
				std::cout << "Query executed successfully!" << std::endl;
				return;
			}

			if (std::get<0>(queryResult) == DatabaseQueryStatusCode::INVALID_QUERY) {
				std::cout << "Failed to interpret query!" << std::endl;
				return;
			}

			if (std::get<0>(queryResult) == DatabaseQueryStatusCode::TABLE_ALREADY_EXISTS) {
				std::cout << "Table already exists" << std::endl;
				return;
			}

			if (std::get<0>(queryResult) == DatabaseQueryStatusCode::DATABASE_ALREADY_EXISTS) {
				std::cout << "Database already exists!" << std::endl;
				return;
			}

			if (std::get<0>(queryResult) == DatabaseQueryStatusCode::DATABASE_NOT_EXISTS) {
				std::cout << "Database does not exist!" << std::endl;
				return;
			}

		}, std::nullopt),
		MenuEntry("4.Exit", []() {
			exit(0);
		},std::nullopt)
	});

	CLIMenu cliMenu(rootPage);

	cliMenu.ShowMenu();

	return cliMenu;
}

int main(int argc, char* argv[])
{
	Engine engine(ENGINE_VERSION);

	engine.Initialize();

	CLIMenu cliMenu = InitializeMenu(engine);

	cliMenu.ShowMenu();
}