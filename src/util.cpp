#include "util.hpp"

#include <charconv>
#include <system_error>

namespace inventory {

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> words;
    std::string current;
    bool in_quotes = false;
    bool has_word = false;
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
            has_word = true;
        } else if ((c == ' ' || c == '\t') && !in_quotes) {
            if (has_word) {
                words.push_back(current);
                current.clear();
                has_word = false;
            }
        } else {
            current.push_back(c);
            has_word = true;
        }
    }
    if (has_word) {
        words.push_back(current);
    }
    return words;
}

template <typename T>
static bool parse_number(const std::string& text, T& out) {
    if (text.empty()) {
        return false;
    }
    T value{};
    const char* begin = text.data();
    const char* end = begin + text.size();
    auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc() || result.ptr != end) {
        return false;
    }
    out = value;
    return true;
}

bool parse_int(const std::string& text, int& out) {
    return parse_number(text, out);
}

bool parse_i64(const std::string& text, std::int64_t& out) {
    return parse_number(text, out);
}

std::string format_money(std::int64_t cents) {
    const bool negative = cents < 0;
    const std::int64_t abs_cents = negative ? -cents : cents;
    const std::int64_t whole = abs_cents / 100;
    const std::int64_t part = abs_cents % 100;
    std::string text = std::string(negative ? "-" : "") + std::to_string(whole) + ".";
    if (part < 10) {
        text += "0";
    }
    text += std::to_string(part);
    return text;
}

}  // namespace inventory
