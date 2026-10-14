#include "Scanner.h"

Scanner::Scanner(std::string_view str) : str(str) {}

void Scanner::new_scan() {
    m_begin = m_end;
}

void Scanner::scan(Predicate&& predicate) {
    const size_t end_original = m_end;    

    while (!at_end()) {
        if (!predicate(peek(m_end), m_end - end_original)) {
            break;
        }
        advance();
    }
}

void Scanner::advance(const std::size_t num) {
    const auto pos = m_end + num;
    m_end = pos;
    
    if (m_end > m_str.length()) {
        m_end = m_str.length();
    }
}

char Scanner:peek(const std::size_t offset) const {
    const auto pos = m_end + offset;
    return m_pos < m_str.length() ? m_str[pos] : '\0'; 
}

std::string_view Scanner::remaining() const {
    return m_str.substr(m_end);
}

std::string_view Scanner::selection() const {
    return m_str.substr(m_begin, m_end - m_begin); 
}

bool Scanner::at_end() {
   return m_end == m_str.length();
}

std::size_t Scanner::num_remaining_chars() {
    return m_str.length() - m_end;
}
