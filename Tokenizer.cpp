#include "Tokenizer.h"

#include <algorithm>
#include <iostream>

Tokenizer::Tokenizer(const std::string_view source) : source(source) {
    using enum TokenType;

    keywords["function"] = Function;
    keywords["const"]   = Const;
    keywords["var"]     = Var;
    keywords["unsafe"]  = Unsafe;
    keywords["if"]      = If;
    keywords["else"]    = Else;
    keywords["elif"]    = Elif;
    keywords["then"]    = Then;
    keywords["for"]     = For;
    keywords["ranged"]  = Ranged;
    keywords["in"]      = In;
    keywords["enum"]    = Enum;
    keywords["bitenum"] = BitEnum;

    symbols[":"]    = Colon;
    symbols[","]    = Comma;
    symbols["="]    = Equal;
    symbols["("]    = LeftParen;
    symbols[")"]    = RightParen;
    symbols["["]    = LeftSquareBracket;
    symbols["]"]    = RightSquareBracket;
    symbols["{"]    = LeftBracket;
    symbols["}"]    = RightBracket;


    symbols["."]    = Dot;
    symbols["..."]  = TripleDot;

    symbols["<"]    = LessThan;
    symbols["<="]   = LessThanOrEqual;
    symbols["=="]   = EqualEqual;
    symbols[">"]    = GreaterThan;
    symbols[">="]   = GreaterThanOrEqual;
    symbols["!"]    = Bang;
    symbols["~"]    = Tilde;
    symbols["<<"]   = BitshiftLeft;
    symbols[">>"]   = BitshiftRight;

    symbols["+"]    = Plus;
    symbols["-"]    = Minus;
    symbols["*"]    = Star;
    symbols["/"]    = Slash;
    symbols["+="]   = PlusEqual;
    symbols["-="]   = MinusEqual;
    symbols["*="]   = StarEqual;
    symbols["/="]   = SlashEqual;
}

void Tokenizer::set_source(SourceFile* source) {
    if (source == nullptr) {
        throw std::invalid_argument("source");
    }
    
    m_source = source;
}

bool Tokenizer::in_identifier(const char ch, const std::size_t index) {
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch == '_') return true;
    return false;
}

bool Tokenizer::in_dollar_identifier(const char ch, const std::size_t index) {
    return in_identifier(ch, index) || (index == 0 && ch == '$');
}

bool Tokenizer::in_number(const char ch, const std::size_t index) {
    return ch >= '0' && ch <= '9';
}

bool Tokenizer::is_symbol(const char ch) {
    switch (ch) {
        case ':':
        case ',':
        case '=':
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
        case '<':
        case '>':
        case '!':
        case '~':
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
    }

    return false;
}

bool Tokenizer::scan_symbol() {
    
}

void Tokenizer::push_token(const TokenType type) {
    
}

std::vector<Token> Tokenizer::tokenize() {
    tokens.clear();
    tokens.shrink_to_fit();
    
    Scanner scanner (source_file->source_view());

    while (!scanner.at_end()) {
        scanner.new_scan();

        if (scan_symbol()) {
            continue;
        }

        if (const char ch = scanner.peek(); in_identifier(ch, 0)) {
            scanner.scan(in_identifier);
            auto type = TokenType::Identifier;
            if (const auto keyword_iter = keywords.find({scan.scanned_str()}); keyword_iter != keywords.end()) {
                type = keyword_iter->second;
            }
            push_token(type);
        }
        else if (in_number(ch, 0)) {
            scanner.scan(in_number);
            push_token(TokenType::LiteralInteger);
        }
        else if (ch == '$') {
            scanner.scan(in_dollar_identifier);
            push_token(TokenType::DollarIdentifier);
        }
        else {
            scanner.advance();
        }
    }

    return tokens;
}
