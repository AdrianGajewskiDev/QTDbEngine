#pragma once
#include <string>
#include <functional>
#include <optional>
#include <memory>

class MenuPage;

class MenuEntry {
public:
	MenuEntry(const std::string& title,
		std::optional<std::function<void()>> action,
		std::optional<std::shared_ptr<MenuPage>> nextPage
	) : m_title(title), m_action(action), m_nextPage(nextPage) {}
	
	void RunAction();
	void Show();
	bool HasNextPage();
	std::shared_ptr<MenuPage> GetNextPage();
private:
	std::string m_title;
	std::optional<std::function<void()>> m_action;
	std::optional<std::shared_ptr<MenuPage>> m_nextPage;
};
