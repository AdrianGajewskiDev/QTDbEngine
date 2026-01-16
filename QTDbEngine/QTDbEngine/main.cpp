#include "pch.h"
#include <iostream>
#include "CLIMenu.h"
#include <memory>

const std::string ENGINE_VERSION = "0.0.1";

int main(int argc, char* argv[])
{
	Engine engine(ENGINE_VERSION);

	engine.Initialize();

	MenuPage rootPage(std::vector<MenuEntry> {
		MenuEntry("1.List Databases", std::nullopt, std::nullopt),
		MenuEntry("2.Create New Database", std::nullopt, std::nullopt),
		MenuEntry("3.Execute SQL", std::nullopt, std::nullopt),
		MenuEntry("4.Exit", []() {
			exit(0);
		}, std::nullopt)

	});

	CLIMenu cliMenu(rootPage);

	cliMenu.ShowMenu();
}
