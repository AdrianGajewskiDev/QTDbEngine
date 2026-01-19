#pragma once
#include "pch.h"
#include <ranges>
#include <string>
#include <vector>
#include <stack>
#include <type_traits>
#include <cstddef>

inline std::vector<std::string> Split(const std::string& str, char delimiter) {
    auto view = str | std::views::split(delimiter)
        | std::views::transform([](auto&& range) {
        return std::string(range.begin(), range.end());
            });

    return std::vector<std::string>(view.begin(), view.end());
}

template<typename T>
inline std::ptrdiff_t GetTokenPosition(size_t occurrence, T token, const std::vector<Token>& tokens)
{
    if (tokens.empty() || occurrence == 0) {
        return -1;
    }

    size_t found = 0;

    for (size_t currIndex = 0; currIndex < tokens.size(); ++currIndex)
    {
        const Token& currentToken = tokens[currIndex];

        // Quick filter by token category based on T
        if constexpr (std::is_same_v<T, SQLOperator>) {
            if (currentToken.Type != SQLTokenType::OPERATOR) continue;
        }
        else if constexpr (std::is_same_v<T, SQLKeyword>) {
            if (currentToken.Type != SQLTokenType::KEYWORD) continue;
        }

        if (!std::holds_alternative<T>(currentToken.Value)) {
            continue;
        }

        if (std::get<T>(currentToken.Value) == token) {
            ++found;
            if (found == occurrence) {
                return static_cast<std::ptrdiff_t>(currIndex);
            }
        }
    }

    return -1;
}

template<typename T>
bool IsVariantType(const auto& variant, const T& value) {
    auto* ptr = std::get_if<T>(&variant);
    return ptr && *ptr == value;
}