#include "Tokenizer.h"

#include <algorithm>
#include <iostream>

bool Tokenizer::in_identifier(const char ch, const std::size_t index) {
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch == '_') return true;
    if (index != 0 && isdigit(ch)) return true;
    return false;
}

bool Tokenizer::in_dollar_identifier(const char ch, const std::size_t index) {
    return in_identifier(ch, index) || (index == 0 && ch == '$');
}

bool Tokenizer::in_number(const char ch, const std::size_t index) {
    return ch >= '0' && ch <= '9';
}

bool Tokenizer::is_symbolic_char(const char ch) {
    return m_symchars.contains(ch);
}

void Tokenizer::load_symbol(const std::string_view sym, const TokenType type) {
    m_symbols[sym] = type;
    for (const char ch : sym) {
        m_symchars.emplace(ch);
    }
}

void Tokenizer::scan_symbol() {
    std::size_t symbol_length = std::min(3uz, m_scanner->num_remaining_chars());
 
    // Find the maximal symbol length
    for (std::size_t offset = 0; offset < symbol_length; ++offset) {
        if (!is_symbolic_char(m_scanner->peek(offset))) {
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
            .text = 
                type == TokenType::EndOfStatement ? 
                "<EOS>" : 
                std::string(m_scanner->selection()),
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

    const auto lm_in_identifier = [&](const char ch, size_t idx) {
        return in_identifier(ch, idx);
    };

    const auto lm_in_dollar_identifier = [&](const char ch, size_t idx) {
        return in_dollar_identifier(ch, idx);
    };

    const auto lm_in_number = [&](const char ch, size_t idx) {
        return in_number(ch, idx);
    };

    while (!m_scanner->at_end()) {
        m_scanner->new_scan();

        if (const char ch = m_scanner->peek(); in_identifier(ch, 0)) {
            m_scanner->scan(lm_in_identifier);
            auto type = TokenType::Identifier;
            if (const auto keyword_iter = m_keywords.find({m_scanner->selection()}); keyword_iter != m_keywords.end()) {
                type = keyword_iter->second;
            }
            push_token(type);
        }
        else if (is_symbolic_char(ch)) {
            scan_symbol();
        }
        else if (in_number(ch, 0)) {
            m_scanner->scan(lm_in_number);
            push_token(TokenType::LiteralInteger);
        }
        else if (ch == '$') {
            m_scanner->scan(lm_in_dollar_identifier);
            push_token(TokenType::DollarIdentifier);
        }
        else if (ch == '#') {   // Comment
            // read until new line or end of file
            while (!m_scanner->at_end()) {
                if (m_scanner->peek() == '\n') break;
                m_scanner->advance();
            }
        }
        else if (ch == '\n' || ch == ';') {
            bool has_duplicate = false;

            if (!m_tokens.empty()) {
                if (m_tokens.back().type == TokenType::EndOfStatement) {
                    has_duplicate = true;
                }
            }
            
            if  (!has_duplicate) {
                push_token(TokenType::EndOfStatement);
            }

            m_scanner->advance();
        }
        else {
            m_scanner->advance();
        }
    }

    return m_tokens;
}

void Tokenizer::load_keywords(std::span<const std::pair<std::string_view, TokenType>> keywords) {
    m_keywords.clear();
    for (const auto& [text, type] : keywords) {
        m_keywords[text] = type;
    }
}

void Tokenizer::load_symbols(std::span<const std::pair<std::string_view, TokenType>> symbols) {
    m_symbols.clear();
    m_symchars.clear();
    for (const auto& [text, type] : symbols) {
        load_symbol(text, type);
    }
}
