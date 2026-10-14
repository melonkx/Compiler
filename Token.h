#pragma once

#include "SourceLocation.h"

enum class TokenType {
    // Reserved Words:
    Function,
    Const,
    Var,
    Unsafe,
    // Constructs
    If,
    Else,
    Elif,
    Then,
    For,
    Ranged,
    In,
    Enum,
    BitEnum,
    // Literals
    LiteralInteger,
    LiteralString,

    //
    Identifier,
    DollarIdentifier,

    // Punctuation
    Colon,                  // :
    Comma,                  // ,
    Equal,                  // =
    LeftParen,              // (
    RightParen,             // )
    LeftBracket,            // {
    RightBracket,           // }
    LeftSquareBracket,      // [
    RightSquareBracket,     // ]
    Dot,                    // .
    TripleDot,              // ...
    FunctionArrow,          // =>

    // Logical symbols
    LessThan,               // <
    LessThanOrEqual,        // <=
    EqualEqual,             // ==
    GreaterThan,            // >
    GreaterThanOrEqual,     // >=
    Bang,                   // !
    Tilde,                  // ~
    BitshiftLeft,           // <<
    BitshiftRight,          // >>

    // Math symbols
    Plus,                   // +
    Minus,                  // -
    Star,                   // *
    Slash,                  // /
    PlusEqual,              // +=
    MinusEqual,             // -=
    StarEqual,              // *=
    SlashEqual,             // /=
};

struct Token {
    TokenType type;
    std::string text;
    SourceLocation location;
};
