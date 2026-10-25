#include "Scanner.h"
#include <stdexcept>

Scanner::Scanner(std::string_view str) : m_str(str) {}

void Scanner::new_scan() {
    m_begin = m_end;
}

void Scanner::scan(Predicate&& predicate) {
    const size_t starting_end_index = end_index();    

    while (!at_end()) {
        if (!predicate(peek(), end_index() - starting_end_index)) {
            break;
        }
        advance();
    }
}

void Scanner::advance(const std::size_t num) {
    if (m_str.empty()) return;

    const auto pos = end_index() + num;
    
    if (pos > m_str.length()) {
        throw std::out_of_range("in Scanner::advance() end_index() + num > m_str.length()");
    }
    
    adjust_cursor(num);
    m_end.index = pos;
}

char Scanner::peek(const std::size_t offset) const {
    const auto pos = end_index() + offset; 
    return pos < m_str.length() ? m_str[pos] : '\0'; 
}

void Scanner::back(const std::size_t offset) {
    if (offset > end_index()) {
        throw std::out_of_range("offset > end_index()");
    } 

    m_end.index -= offset;
}

std::string_view Scanner::remaining() const {
    return m_str.substr(end_index());
}

std::string_view Scanner::selection() const {
    return m_str.substr(m_begin.index, m_end.index - m_begin.index); 
}

bool Scanner::at_end() const {
   return end_index() == m_str.length();
}

std::size_t Scanner::num_remaining_chars() const {
    return m_str.length() - end_index();
}

std::size_t Scanner::line() const {
    return m_begin.line;
}

std::size_t Scanner::column() const {
    return m_begin.column;
}

std::size_t Scanner::end_index() const {
    return m_end.index;
}

void Scanner::adjust_cursor(std::size_t steps) {
    return;
    for (std::size_t offset = 0; offset < steps; ++offset) {
        const char ch = peek(offset);

        if (ch == '\n') {
            m_end.line++;
            m_end.column = 1;
        } else {
            m_end.column++;
        }
    }


}
