#include "Tokenizer.h"

#include <algorithm>
#include <iostream>

Tokenizer::Tokenizer() {
    using enum TokenType;

    m_keywords["function"] = Function;
    m_keywords["const"]   = Const;
    m_keywords["var"]     = Var;
    m_keywords["object"]  = Object;
    
    m_keywords["unsafe"]  = Unsafe;
    m_keywords["if"]      = If;
    m_keywords["else"]    = Else;
    m_keywords["elif"]    = Elif;
    m_keywords["then"]    = Then;
    m_keywords["return"]  = Return;
    m_keywords["for"]     = For;
    m_keywords["ranged"]  = Ranged;
    m_keywords["in"]      = In;
    m_keywords["enum"]    = Enum;
    m_keywords["bitenum"] = BitEnum;

    m_symbols[":"]    = Colon;
    m_symbols[","]    = Comma;
    m_symbols["="]    = Equal;
    m_symbols["("]    = LeftParen;
    m_symbols[")"]    = RightParen;
    m_symbols["["]    = LeftSquareBracket;
    m_symbols["]"]    = RightSquareBracket;
    m_symbols["{"]    = LeftBracket;
    m_symbols["}"]    = RightBracket;
    m_symbols["@"]    = At;

    m_symbols["^"]    = Hat;
    m_symbols["&"]    = Ampersand;
    m_symbols["|"]     = Pipe;

    m_symbols["."]    = Dot;
    m_symbols["..."]  = TripleDot;
    m_symbols["=>"]   = FunctionArrow;
    m_symbols["!!"]   = BangBang;

    m_symbols["<"]    = LessThan;
    m_symbols["<="]   = LessThanOrEqual;
    m_symbols["=="]   = EqualEqual;
    m_symbols[">"]    = GreaterThan;
    m_symbols[">="]   = GreaterThanOrEqual;
    m_symbols["!"]    = Bang;
    m_symbols["~"]    = Tilde;
    m_symbols["<<"]   = BitshiftLeft;
    m_symbols[">>"]   = BitshiftRight;

    m_symbols["+"]    = Plus;
    m_symbols["-"]    = Minus;
    m_symbols["*"]    = Star;
    m_symbols["/"]    = Slash;
    m_symbols["+="]   = PlusEqual;
    m_symbols["-="]   = MinusEqual;
    m_symbols["*="]   = StarEqual;
    m_symbols["/="]   = SlashEqual;
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
        case '.':
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

void Tokenizer::set_symbol(const std::string_view sym, const TokenType type) {
    m_symbols[sym] = type;
    for (const char ch : sym) {
        m_symchars.try_emplace(ch);
    }
}

void Tokenizer::scan_symbol() {
    std::size_t symbol_length = std::min(3uz, m_scanner->num_remaining_chars());
 
    // Find the maximal symbol length
    for (std::size_t offset = 0; offset < symbol_length; ++offset) {
        if (!is_symbol(m_scanner->peek(offset))) {
            --symbol_length;
        }
    }

    m_scanner->advance(symbol_length);

    // Find the largest symbol
    for (std::size_t length = 0; length < symbol_length; ++length) {
        const auto symbol = m_scanner->selection();

        if (auto it = m_symbols.find({symbol}); it != m_symbols.end()) {
            push_token(it->second);
            return;
        }

        m_scanner->back();
    }
    
    // TODO: The symbol was unexpected
    m_scanner->advance();
}

void Tokenizer::push_token(const TokenType type) {
    m_tokens.push_back(Token{
            .type = type, 
            .text = std::string(m_scanner->selection()),
            .location = {
               .line = m_scanner->line(),
               .column = m_scanner->column(),
               .path = std::string(m_source_file->path())
            }
        });
}

std::vector<Token> Tokenizer::tokenize(const SourceFile& source_file) {
    Scanner scanner (source_file.contents());
  
    m_tokens.clear();
    m_tokens.shrink_to_fit();
     
    // Reinitialize pointers
    m_scanner = &scanner;
    m_source_file = &source_file;

    while (!m_scanner->at_end()) {
        m_scanner->new_scan();

        if (const char ch = m_scanner->peek(); in_identifier(ch, 0)) {
            m_scanner->scan(in_identifier);
            auto type = TokenType::Identifier;
            if (const auto keyword_iter = m_keywords.find({m_scanner->selection()}); keyword_iter != m_keywords.end()) {
                type = keyword_iter->second;
            }
            push_token(type);
        }
        else if (is_symbol(ch)) {
            scan_symbol();
        }
        else if (in_number(ch, 0)) {
            m_scanner->scan(in_number);
            push_token(TokenType::LiteralInteger);
        }
        else if (ch == '$') {
            m_scanner->scan(in_dollar_identifier);
            push_token(TokenType::DollarIdentifier);
        }
        else {
            m_scanner->advance();
        }
    }

    return m_tokens;
}
