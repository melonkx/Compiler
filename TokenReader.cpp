#include "TokenReader.h"


TokenReader::TokenReader(const std::span<const Token> tokens) : tokens{tokens}, index{0} {}

bool TokenReader::at_end() const {
    return index == tokens.size();
}

bool TokenReader::is(const TokenType type) const {
    return !at_end() && current().type == type;
}

bool TokenReader::consume(TokenType type) {
    const bool matches = is(type);
    if (matches) {
        ++index;
    }
    return matches;
}

const Token& TokenReader::current() const {
    return tokens[index];
}

const Token& TokenReader::advance() {
    const Token& token = current();
    ++index;
    return token;
}

