#pragma once

#include "SourceLocation.h"

enum class TokenType {
    Function,
    Const,
    Var,
    Object,
    If,
    Else,
    Elif,
    Then,
    Return,
    As,

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
    DoubleDot,              // ..
    Arrow,                  // =>

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


    // NOTE: Automatically added at the end of a line
    EndOfStatement                     // ;
};

struct Token {
    TokenType type;
    std::string text;
    SourceLocation location;
};
