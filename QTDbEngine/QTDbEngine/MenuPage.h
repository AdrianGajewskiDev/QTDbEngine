#pragma once
#include "MenuEntry.h"
#include <memory>

class MenuPage {
public:
	MenuPage(std::vector<MenuEntry> pageEntrries) : m_PageEntries(pageEntrries) {};

	std::optional<std::shared_ptr<MenuPage>> ExecAction(int8_t index);
	void RenderPage();
	int8_t GetEntriesSize();
private:
	std::vector<MenuEntry> m_PageEntries;
};