#include "MenuEntry.h"
#include <iostream>

void MenuEntry::RunAction()
{
	m_action.value()();
}

void MenuEntry::Show()
{
	std::cout << m_title << std::endl;
}

bool MenuEntry::HasNextPage()
{
	return m_nextPage.has_value();
}

std::shared_ptr<MenuPage> MenuEntry::GetNextPage()
{
	return m_nextPage.value();
}