#include "TokenReader.h"


TokenReader::TokenReader(const std::span<const Token> tokens) 
    : m_tokens{tokens},
      m_index{}
{}

bool TokenReader::at_end() const {
    return m_index == m_tokens.size();
}

bool TokenReader::is(const TokenType type) const {
    return !at_end() && current().type == type;
}

bool TokenReader::consume(TokenType type) {
    const bool matches = is(type);
    if (matches) {
        ++m_index;
    }
    return matches;
}

const Token& TokenReader::current() const {
    return m_tokens[m_index];
}

const Token& TokenReader::advance() {
    const Token& token = current();
    ++m_index;
    return token;
}

