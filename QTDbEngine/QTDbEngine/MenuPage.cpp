#include "MenuPage.h"

std::optional<std::shared_ptr<MenuPage>> MenuPage::ExecAction(int8_t index)
{
	MenuEntry* entry = &m_PageEntries[index];

	if (entry->HasNextPage()) {
		return entry->GetNextPage();
	}

	entry->RunAction();

	return std::nullopt;
}

void MenuPage::RenderPage()
{
	for (auto& entry : m_PageEntries) {
		entry.Show();
	}
}

int8_t MenuPage::GetEntriesSize() 
{
	return m_PageEntries.size();
}