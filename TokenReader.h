#pragma once

#include "Token.h"
#include <span>

class TokenReader {
public:
    explicit TokenReader(std::span<const Token> tokens);    

    bool at_end() const;
    bool is(TokenType type) const;
    bool consume(TokenType type);

    const Token& current() const;
    const Token& advance();

private:
    std::span<const Token> m_tokens;
    std::size_t m_index;
};
