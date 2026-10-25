#pragma once
#include <functional>
#include <string>

class Scanner {
public:
    explicit Scanner(std::string_view str);        

public:
    using Predicate = std::function<bool(char, std::size_t)>;

    void new_scan();
    void scan(Predicate&& predicate);
    void advance(std::size_t num = 1);
    char peek(std::size_t offset = 0) const;
    void back(std::size_t offset = 1);
    std::string_view remaining() const;
    std::size_t num_remaining_chars() const;
    std::string_view selection() const;
    bool at_end() const;
    std::size_t line() const;
    std::size_t column() const;

private:
    std::size_t end_index() const;
    void adjust_cursor(std::size_t steps);

private:
    struct Cursor {
        std::size_t index {};
        std::size_t line {1};
        std::size_t column {1};
    };

    std::string_view m_str{};
    Cursor m_begin;
    Cursor m_end;
};
