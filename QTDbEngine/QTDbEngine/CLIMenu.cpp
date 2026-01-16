#include "CLIMenu.h"
#include <iostream>

CLIMenu::CLIMenu(MenuPage firstPage) : isRunning(false) {
	m_MenuPagesStack.push(std::make_shared<MenuPage>(firstPage));
}

void CLIMenu::ShowMenu()
{
	isRunning = true;

	while (isRunning) {
		std::shared_ptr<MenuPage> top = m_MenuPagesStack.top();

		top->RenderPage();

		std::cout << "Select an option (9 to go back): ";
		int choice;
		std::cin >> choice;

		if (choice == 9) {
			if (m_MenuPagesStack.size() > 1) {
				m_MenuPagesStack.pop();
			}
			continue;
		}

		if (choice > top->GetEntriesSize() || choice < 1) {
			std::cout << "Invalid option. Try again!" << std::endl;
			continue;
		}

		std::optional<std::shared_ptr<MenuPage>> nextPage = top->ExecAction(choice - 1);

		if (nextPage.has_value()) {
			m_MenuPagesStack.push(nextPage.value());
		}
	}
}