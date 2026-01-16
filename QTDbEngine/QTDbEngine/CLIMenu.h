#pragma once
#include <stack>
#include <memory>
#include "MenuPage.h"

class CLIMenu {
public:
	CLIMenu(MenuPage firstPage);
	void ShowMenu();
private:
	std::stack<std::shared_ptr<MenuPage>> m_MenuPagesStack;
	bool isRunning;
};