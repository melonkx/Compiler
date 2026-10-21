#pragma once

#include "SourceLocation.h"

enum class TokenType {
    // Reserved Words:
    Function,
    Const,
    Var,
    Unsafe,
    Object,

    // Constructs
    If,
    Else,
    Elif,
    Then,
    Return,
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
    At,                     // @
    Dot,                    // .
    TripleDot,              // ...
    FunctionArrow,          // =>
    BangBang,               // !!

    // Logical symbols
    LessThan,               // <
    LessThanOrEqual,        // <=
    EqualEqual,             // ==
    GreaterThan,            // >
    GreaterThanOrEqual,     // >=
    Bang,                   // !
    BitshiftLeft,           // <<
    BitshiftRight,          // >>
    Tilde,                  // ~
    Ampersand,              // &
    Pipe,                   // |
    Hat,                    // ^

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
