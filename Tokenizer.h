#pragma once

#include <vector>
#include <unordered_map>

#include "Token.h"
#include "Scanner.h"
#include "SourceFile.h"


class Tokenizer {
public:
    explicit Tokenizer();
    [[nodiscard]] std::vector<Token> tokenize(const SourceFile& source_file);

private:
    static bool in_identifier(char ch, std::size_t index);
    static bool in_dollar_identifier(char ch, std::size_t index);
    static bool in_number(char ch, std::size_t index);
    static bool is_symbol(char ch);

private:
    void scan_symbol();
    void push_token(TokenType type);
    void set_symbol(std::string_view sym, TokenType type);

private:
    std::vector<Token> m_tokens;
    Scanner* m_scanner;
    const SourceFile* m_source_file;
    std::unordered_map<std::string_view, TokenType> m_keywords;
    std::unordered_map<std::string_view, TokenType> m_symbols;
    std::set<char> m_symchars;
};
 
