#pragma once

class Scanner {
public:
    explicit Scanner(std::string_view str);        

public:
    using Predicate = std::function<bool(char, std::size_t)>;

    void new_scan();
    void scan(Predicate&& predicate);
    void advance(std::size_t num = 1);
    char peek(std::size_t offset = 0) const;
    std::string_view remaining() const;
    std::string_view selection() const;
    bool at_end() const;
    std::size_t line() const;
    std::size_t column() const;

private:
    std::string_view m_str{};

    struct Cursor {
        std::size_t index;
        std::size_t line;
        std::size_t column;
    };

    Cursor m_begin;
    Cursor m_end;

};
